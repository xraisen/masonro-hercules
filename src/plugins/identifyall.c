#include <stdio.h>
#include <string.h>
#include "../common/HPMi.h"
#include "../map/script.h"
#include "../map/clif.h"
#include "../map/pc.h"
#include "../map/atcommand.h"

#include "../common/HPMDataCheck.h"    //Should always be the last include file.

/*
~ 1.0 Initial Script
~ 2.0 Update compilation compatibility Hercules Rev. 13300
*/
HPExport struct hplugin_info pinfo = {
        "identifyall",	// Plugin name
        SERVER_TYPE_MAP,// Which server types this plugin works with?
        "2.0",          // Plugin version
        HPM_VERSION,    // HPM Version (don't change, macro is automatically updated)
};

ACMD(iall)
{
    int i,num;
    struct item it;

	if (!sd) return false;

    for(i=num=0;i < MAX_INVENTORY;i++){
        if(!sd->status.inventory[i].identify && sd->status.inventory[i].nameid){
            it=sd->status.inventory[i];
            pc->delitem(sd,i,it.amount,0,0, LOG_TYPE_COMMAND);
            it.identify=1;
            pc->additem(sd,&it,it.amount, LOG_TYPE_COMMAND);
            num++;
        }
    }
    clif->message(fd,(num) ? "All items have been identified." : atcommand->msg_table[1238]);
  
    return true;
}

/* Server Startup */
HPExport void plugin_init (void)
{
    clif = GET_SYMBOL("clif");
    script = GET_SYMBOL("script");
    pc = GET_SYMBOL("pc");
    atcommand = GET_SYMBOL("atcommand");

    addAtcommand("iall",iall);
}