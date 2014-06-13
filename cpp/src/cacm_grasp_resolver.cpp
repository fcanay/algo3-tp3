#include "cacm.h"
#include "cacm_goloso.h"
#include "cacm_busq_local.h"
#include "cacm_grasp.h"
using namespace cacm;
using namespace cacm_grasp;

#include <iostream>
#include <algorithm>
using namespace std;



#define range(c) (c).begin(), (c).end()
salida cacm_grasp::resolver(const entrada& e)
{ 
	int max_fallos = 20;
	double epsilon = 0.1;
	double p;
  salida mejor_s = cacm_goloso::resolver(e,20,&p);
	if(!mejor_s.hay_solucion) {return mejor_s;}
	int fallos = 0;
	salida s;
  while(fallos < max_fallos)
  {
  	solucion_aleatoria(e,s,p);
  	cacm_busq_local::busqueda_local(e,s);
  	if(!s.hay_solucion) 
  	{
  	fallos++;
  	continue;
  	}
  	
  	if(error_relativo(mejor_s,s) < epsilon)
  	{
  		fallos++;
  	}
  	else
  	{
  		fallos = 0;
  	}
  }
  return s;
}

void cacm_grasp::solucion_aleatoria(const entrada& e, salida& s, double p)
{
	vector<bool> visitado(e.n + 1);
  vector<double> dist(e.n + 1);
  vector<int> dist_w1(e.n + 1);
  vector<int> dist_w2(e.n + 1);
  vector<int> pred(e.n + 1);
  
  dijkstra(e, visitado, dist, dist_w1, dist_w2, pred, p);
  
  s.hay_solucion = true;
      
  for (int v = e.v; pred[v] != -1; v = pred[v])
  {
    int w1 = dist_w1[v] - dist_w1[pred[v]],
    		w2 = dist_w2[v] - dist_w2[pred[v]];
    s.ejes.push_front(eje(pred[v], v, w1, w2));
  }
  s.u = e.u;
  s.v = e.v;
  s.W1 = dist_w1[e.v];
  s.W2 = dist_w2[e.v];
}

double error_relativo(salida& mejor_s,salida& s)
{
	return abs(mejor_s.W2-s.W2) / mejor_s.W2;
}