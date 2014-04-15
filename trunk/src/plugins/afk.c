#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../common/HPMi.h"
#include "../common/timer.h"
#include "../map/script.h"
#include "../map/pc.h"
#include "../map/clif.h"
#include "../map/battle.h"
#include "../map/status.h"
#include "../common/HPMDataCheck.h"


/*
1.0 Initial Script [Mhalicot]
    Topic: http://goo.gl/vbmQnr
2.0 Added Map restriction "izlude", GVG/PVP.
    You can't use @afk when your dead. [Mhalicot]
2.1 Update compilation compatibility in latest
    rev. 13300  [Mhalicot]
2.2 Fixed bug when using @afk. [Mhalicot]
2.3 Fixed map crash when using @afk [Mhalicot]
3.0 Added unable to use @afk when receiving damage. [Mhalicot]
*/

HPExport struct hplugin_info pinfo = {
        "afk",                // Plugin name
        SERVER_TYPE_MAP,// Which server types this plugin works with?
        "3.0",                        // Plugin version
        HPM_VERSION,        // HPM Version (don't change, macro is automatically updated)
};
ACMD(afk) {
    if(sd->bl.m == map->mapname2mapid("izlude")) {
        clif->message(fd, "@afk is not allowed on this map.");
        return true;
    }
    if( pc_isdead(sd) ) {
        clif->message(fd, "Cannot use @afk if you are dead.");
        return true;
    }

    if(DIFF_TICK(timer->gettick(),sd->canlog_tick) < battle->bc->prevent_logout) {
        clif->message(fd, "Failed to use @afk, please try again later."); //<- (10s)10000ms delay to edit look for conf/battle/player.conf search for prevent_logout
        return true;
    }

    if( map->list[sd->bl.m].flag.autotrade == battle->bc->autotrade_mapflag )
    {

        if(map->list[sd->bl.m].flag.pvp || map->list[sd->bl.m].flag.gvg){
            clif->message(fd, "You may not use the @afk maps PVP or GVG.");
        return true;
        }
        sd->state.autotrade = 1;
        sd->state.monster_ignore = 1;
        pc_setsit(sd);
        skill->sit(sd,1);
        clif->sitting(&sd->bl);
        clif->changelook(&sd->bl,LOOK_HEAD_TOP,471);
        clif->specialeffect(&sd->bl, 234,AREA);              
        if( battle->bc->at_timeout )
            {
            int timeout = atoi(message);
            status->change_start(NULL, &sd->bl, SC_AUTOTRADE, 10000, 0, 0, 0, 0, ((timeout > 0) ? min(timeout,battle->bc->at_timeout) : battle->bc->at_timeout) * 60000, 0);
        }
            clif->chsys_quit(sd);
            clif->authfail_fd(sd->fd, 15);
        } else    clif->message(fd, "@afk is not allowed on this map.");
        return true;
}

/* Server Startup */
HPExport void plugin_init (void)
{
    clif = GET_SYMBOL("clif");
    skill = GET_SYMBOL("skill");
    script = GET_SYMBOL("script");
    pc = GET_SYMBOL("pc");
    battle = GET_SYMBOL("battle");
    map = GET_SYMBOL("map");
    status = GET_SYMBOL("status");
    timer = GET_SYMBOL("timer");

    addAtcommand("afk",afk);
}