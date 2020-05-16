#include "Graph.h"

void UGraph::init()
{
	memset(head, -1, sizeof(head));
	vn = 0;
	en = 0;
}

void UGraph::addv(int id, int label)
{
	vtx[id] = Vertex(id, label);
	vn++;
}

void UGraph::addse(int u, int v, int label)
{
	edge[en] = Edge(u, v, label, head[u]);
	head[u] = en++;
}

void UGraph::adde(int u, int v, int label)
{
	addse(u, v, label);
	addse(v, u, label);
}

void UGraph::delse(int u, int v, int label)
{
	for (int i = head[u]; ~i; i = edge[i].next)
	{
		if (edge[i].u == u && edge[i].v == v && edge[i].label == label)
		{
			edge[i].del = 1;
			return;
		}
	}
}

void UGraph::dele(int u, int v, int label)
{
	for (int i = head[u]; ~i; i = edge[i].next)
	{
		if (edge[i].u == u && edge[i].v == v && edge[i].label == label)
		{
			edge[i].del = 1;
			edge[i ^ 1].del = 1;
			return;
		}
	}
}