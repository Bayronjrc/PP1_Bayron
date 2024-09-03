#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <stddef.h>

typedef struct {
    int venta_id;
    char *fecha;             
    int producto_id;
    char *producto_nombre;  
    char *categoria;         
    int cantidad;
    double precio_unitario;
    double total;
} Venta;


typedef struct {
    Venta *ventas; // Array dinámico de ventas
    size_t tamano; // Número actual de ventas en el array
    size_t capacidad; // Capacidad máxima del array antes de redimensionar
} ListaVentas;

extern ListaVentas listaVentas; // Declaración de la variable listaVentas

void inicializarListaVentas(ListaVentas *lista);
void agregarVenta(ListaVentas *lista, Venta nuevaVenta);
void liberarListaVentas(ListaVentas *lista);
void imprimirListaVentas(const ListaVentas *lista); // Nueva función
void cargarDatosJSON(ListaVentas *lista, const char *nombreArchivo);
void guardarDatosJSON(const ListaVentas *lista, const char *nombreArchivo);

#endif
