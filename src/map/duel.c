// Copyright (c) Hercules Dev Team, licensed under GNU GPL.
// See the LICENSE file
// Portions Copyright (c) Athena Dev Teams

#include "../common/cbasetypes.h"

#include "atcommand.h"  // msg_txt
#include "clif.h"
#include "duel.h"
#include "pc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*==========================================
 * Duel organizing functions [LuzZza]
 *------------------------------------------*/
void duel_savetime(struct map_session_data* sd) {
	time_t clock;
	struct tm *t;
	
	time(&clock);
	t = localtime(&clock);
	
	pc_setglobalreg(sd, script->add_str("PC_LAST_DUEL_TIME"), t->tm_mday*24*60 + t->tm_hour*60 + t->tm_min);
}

int duel_checktime(struct map_session_data* sd) {
	int diff;
	time_t clock;
	struct tm *t;
	
	time(&clock);
	t = localtime(&clock);
	
	diff = t->tm_mday*24*60 + t->tm_hour*60 + t->tm_min - pc_readglobalreg(sd, script->add_str("PC_LAST_DUEL_TIME") );
	
	return !(diff >= 0 && diff < battle_config.duel_time_interval);
}
static int duel_showinfo_sub(struct map_session_data* sd, va_list va)
{
	struct map_session_data *ssd = va_arg(va, struct map_session_data*);
	int *p = va_arg(va, int*);
	char output[256];

	if (sd->duel_group != ssd->duel_group) return 0;
	
	sprintf(output, "      %d. %s", ++(*p), sd->status.name);
	clif_disp_onlyself(ssd, output, strlen(output));
	return 1;
}

void duel_showinfo(const unsigned int did, struct map_session_data* sd) {
	int p=0;
	char output[256];

	if(duel->list[did].max_players_limit > 0)
		sprintf(output, msg_txt(370), //" -- Duels: %d/%d, Members: %d/%d, Max players: %d --"
			did, duel->count,
			duel->list[did].members_count,
			duel->list[did].members_count + duel->list[did].invites_count,
			duel->list[did].max_players_limit);
	else
		sprintf(output, msg_txt(371), //" -- Duels: %d/%d, Members: %d/%d --"
			did, duel->count,
			duel->list[did].members_count,
			duel->list[did].members_count + duel->list[did].invites_count);

	clif_disp_onlyself(sd, output, strlen(output));
	map->foreachpc(duel_showinfo_sub, sd, &p);
}

int duel_create(struct map_session_data* sd, const unsigned int maxpl) {
	int i=1;
	char output[256];
	
	while(duel->list[i].members_count > 0 && i < MAX_DUEL) i++;
	if(i == MAX_DUEL) return 0;
	
	duel->count++;
	sd->duel_group = i;
	duel->list[i].members_count++;
	duel->list[i].invites_count = 0;
	duel->list[i].max_players_limit = maxpl;
	
	strcpy(output, msg_txt(372)); // " -- Duel has been created (@invite/@leave) --"
	clif_disp_onlyself(sd, output, strlen(output));
	
	clif->map_property(sd, MAPPROPERTY_FREEPVPZONE);
	clif->maptypeproperty2(&sd->bl,SELF);
	return i;
}

void duel_invite(const unsigned int did, struct map_session_data* sd, struct map_session_data* target_sd) {
	char output[256];

	// " -- Player %s invites %s to duel --"
	sprintf(output, msg_txt(373), sd->status.name, target_sd->status.name);
	clif->disp_message(&sd->bl, output, strlen(output), DUEL_WOS);

	target_sd->duel_invite = did;
	duel->list[did].invites_count++;
	
	// "Blue -- Player %s invites you to PVP duel (@accept/@reject) --"
	sprintf(output, msg_txt(374), sd->status.name);
	clif->broadcast((struct block_list *)target_sd, output, strlen(output)+1, BC_BLUE, SELF);
}

static int duel_leave_sub(struct map_session_data* sd, va_list va)
{
	int did = va_arg(va, int);
	if (sd->duel_invite == did)
		sd->duel_invite = 0;
	return 0;
}

void duel_leave(const unsigned int did, struct map_session_data* sd) {
	char output[256];
	
	// " <- Player %s has left duel --"
	sprintf(output, msg_txt(375), sd->status.name);
	clif->disp_message(&sd->bl, output, strlen(output), DUEL_WOS);
	
	duel->list[did].members_count--;
	
	if(duel->list[did].members_count == 0) {
		map->foreachpc(duel_leave_sub, did); 
		duel->count--;
	}
	
	sd->duel_group = 0;
	duel_savetime(sd);
	clif->map_property(sd, MAPPROPERTY_NOTHING);
	clif->maptypeproperty2(&sd->bl,SELF);
}

void duel_accept(const unsigned int did, struct map_session_data* sd) {
	char output[256];
	
	duel->list[did].members_count++;
	sd->duel_group = sd->duel_invite;
	duel->list[did].invites_count--;
	sd->duel_invite = 0;
	
	// " -> Player %s has accepted duel --"
	sprintf(output, msg_txt(376), sd->status.name);
	clif->disp_message(&sd->bl, output, strlen(output), DUEL_WOS);

	clif->map_property(sd, MAPPROPERTY_FREEPVPZONE);
	clif->maptypeproperty2(&sd->bl,SELF);
}

void duel_reject(const unsigned int did, struct map_session_data* sd) {
	char output[256];
	
	// " -- Player %s has rejected duel --"
	sprintf(output, msg_txt(377), sd->status.name);
	clif->disp_message(&sd->bl, output, strlen(output), DUEL_WOS);
	
	duel->list[did].invites_count--;
	sd->duel_invite = 0;
}

void do_final_duel(void) {
}

void do_init_duel(bool minimal) {
	if (minimal)
		return;

	memset(&duel->list[0], 0, sizeof(duel->list));
}

/*=====================================
* Default Functions : duel.h 
* Generated by HerculesInterfaceMaker
* created by Susu
*-------------------------------------*/
void duel_defaults(void) {
	duel = &duel_s;
	/* vars */
	duel->count = 0;
	/* funcs */
	//Duel functions // [LuzZza]
	duel->create = duel_create;
	duel->invite = duel_invite;
	duel->accept = duel_accept;
	duel->reject = duel_reject;
	duel->leave = duel_leave;
	duel->showinfo = duel_showinfo;
	duel->checktime = duel_checktime;

	duel->init = do_init_duel;
	duel->final = do_final_duel;
}
