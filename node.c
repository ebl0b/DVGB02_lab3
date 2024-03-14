#include "node.h"

static void printarr(int* arr, int size){
	printf("{");
	for(int i = 0; i<size; i++){
		i+1!=size ? printf("%d, ", arr[i]) : printf("%d}\n", arr[i]);
	}
}

void rtinit(struct distance_table *table, int node)
{
	// Tables are already initialized
	struct rtpkt routing_packet;
	int min_costs[NCOUNT];
	for(int i = 0; i<NCOUNT; i++){
		min_costs[i] = table->costs[node][i];
	}
	//printarr(min_costs, NCOUNT);
	routing_packet.sourceid = node;
	memcpy(routing_packet.mincost, min_costs, NCOUNT*sizeof(int));
	for(int i = 0; i<NCOUNT; i++){
		if(is_neighbor(node, i)){
			routing_packet.destid = i;
			tolayer2(routing_packet);
		}
	}
	printdt(table, node);
}

void rtupdate(struct distance_table *table, int node, struct rtpkt *pkt) {
	int changed = 0;
	for(int i = 0; i<NCOUNT; i++){
		if(pkt->mincost[i] != table->costs[pkt->sourceid][i]){
			table->costs[pkt->sourceid][i] = pkt->mincost[i];
			changed = 1;
		}
		if(pkt->mincost[i] + table->costs[node][pkt->sourceid] < table->costs[node][i]){
			table->costs[node][i] = pkt->mincost[i] + table->costs[node][pkt->sourceid];
		}
	}
	if(changed)
		rtinit(table, node);
	printdt(table, node);
}


