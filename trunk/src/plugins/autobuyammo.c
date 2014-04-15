/*
=============================================
@autobuyammo Plugin
Converted by: Samuel
Original Made by: goddameit
================================================
v 1.0 Initial Release
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../common/HPMi.h"
#include "../common/mmo.h"
#include "../common/malloc.h"
#include "../map/pc.h"
#include "../map/battle.h"
#include "../map/itemdb.h"
#include "../map/script.h"

#include "../common/HPMDataCheck.h"  

HPExport struct hplugin_info pinfo = {
	"autobuyammo",			// Plugin name
	SERVER_TYPE_MAP,	// Which server types this plugin works with?
	"1.0",				// Plugin version
	HPM_VERSION,		// HPM Version (don't change, macro is automatically updated)
};

void my_battle_consume_ammo_preHook(TBL_PC*sd, int *skill_id, int *lv) {

	unsigned int idx = sd->equip_index[EQI_AMMO];
	if((sd->equip_index[EQI_AMMO]>=0) && ( pc->readreg(sd,script->add_str("@SP_AUTOBUYAMMO_GODD")) ))
	{
		unsigned int nameid = sd->status.inventory[idx].nameid;
		unsigned int amount = sd->status.inventory[idx].amount;

		struct item_data *i_data = itemdb->exists(nameid);
		if( amount <= 1 )
		{
			unsigned int nzeny = ( i_data->value_buy + 5 )*10;
			if( !pc->payzeny(sd, nzeny, LOG_TYPE_NONE, NULL) )
			{
				struct item it;
				short flag = -1;
				memset(&it,0,sizeof(it));
				it.nameid=i_data->nameid;
				it.identify=1;
				flag = pc->additem(sd, &it, 9, LOG_TYPE_NONE);
			}
		}
	}
}

/* run when server starts */
HPExport void plugin_init (void) {
	iMalloc = GET_SYMBOL("iMalloc");
	pc = GET_SYMBOL("pc");
	itemdb = GET_SYMBOL("itemdb");
	battle = GET_SYMBOL("battle");
	script = GET_SYMBOL("script");
	
	// Add the hook
	addHookPre("battle->consume_ammo",my_battle_consume_ammo_preHook);
}
/* run when server is ready (online) */
HPExport void server_online (void) {
}
/* run when server is shutting down */
HPExport void plugin_final (void) {
}