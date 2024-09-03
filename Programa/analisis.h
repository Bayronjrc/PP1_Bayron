#ifndef ANALISIS_H
#define ANALISIS_H

#include "data_structures.h"

double calcularTotalVentas(ListaVentas *lista);
void calcularVentasMensualesAnuales(ListaVentas *lista);
void mesConMayorVenta(const ListaVentas *lista);
void diaSemanaMasActivo(const ListaVentas *lista);
double calcularVentasTrimestre(const ListaVentas *lista,int, int);
void calcularTasaCrecimiento(int,int,int,int,const ListaVentas *lista);

#endif
