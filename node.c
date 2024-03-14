#include "node.h"

static void rtsend(int* min_costs, int node){
	struct rtpkt routing_packet;
	routing_packet.sourceid = node;
	memcpy(routing_packet.mincost, min_costs, NCOUNT*sizeof(int));
	for(int i = 0; i<NCOUNT; i++){
		if(is_neighbor(node, i)){
			routing_packet.destid = i;
			tolayer2(routing_packet);
		}
	}
}

static void extract_mincosts(int* min_costs, struct distance_table* table, int node){
	for(int i = 0; i<NCOUNT; i++){
		min_costs[i] = table->costs[node][i];
	}
}

void rtinit(struct distance_table *table, int node)
{
	// Tables are already initialized
	int min_costs[NCOUNT];
	extract_mincosts(min_costs, table, node);
	rtsend(min_costs, node);
}

void rtupdate(struct distance_table *table, int node, struct rtpkt *pkt) {
	int changed = 0;
	int stodcost = table->costs[node][pkt->sourceid];
	for(int i = 0; i<NCOUNT; i++){
		if((pkt->mincost[i] != table->costs[pkt->sourceid][i]))
			table->costs[pkt->sourceid][i] = pkt->mincost[i];
		if(pkt->mincost[i] + stodcost < table->costs[node][i]){
			table->costs[node][i] = pkt->mincost[i] + stodcost;
			changed = 1;
		}
	}
	if(changed){
		int min_costs[NCOUNT];
		extract_mincosts(min_costs, table, node);
		rtsend(min_costs, node);
	}else printdt(table, node);
}


