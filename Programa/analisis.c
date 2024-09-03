#include <stdio.h>
#include <string.h>
#include "analisis.h"
#include <time.h>

#define MAX_YEARS 100 // Define el rango de años que vamos a manejar

// Calcula el total de ventas (suma de los importes de todas las ventas)
double calcularTotalVentas(const ListaVentas *lista) {
    double total = 0.0;
    for (size_t i = 0; i < lista->tamano; i++) {
        total += lista->ventas[i].total;
    }
    return total;
}

// Calcula el total de ventas por mes y año
void calcularVentasMensualesAnuales(const ListaVentas *lista) {
    double ventasMensuales[12] = {0};
    double ventasAnuales[MAX_YEARS] = {0}; // Suponiendo un máximo de 100 años de registros

    for (size_t i = 0; i < lista->tamano; i++) {
        int year, month, day;
        sscanf(lista->ventas[i].fecha, "%d-%d-%d", &year, &month, &day);

        if (month >= 1 && month <= 12 && year >= 2000 && year < 2000 + MAX_YEARS) {
            ventasMensuales[month - 1] += lista->ventas[i].total;
            ventasAnuales[year - 2000] += lista->ventas[i].total;
        } else {
            printf("Anno o mes fuera de rango: %d-%d\n", year, month);
        }
    }

    // Mostrar resultados
    printf("\n--- Ventas Mensuales ---\n");
    for (int i = 0; i < 12; i++) {
        printf("Mes %d: %.2f\n", i + 1, ventasMensuales[i]);
    }

    printf("\n--- Ventas Anuales ---\n");
    for (int i = 0; i < MAX_YEARS; i++) {
        if (ventasAnuales[i] > 0) {
            printf("Anno %d: %.2f\n", i + 2000, ventasAnuales[i]);
        }
    }
}

void mesConMayorVenta(const ListaVentas *lista) {
    double ventasMensuales[12] = {0}; // Array para almacenar el total de ventas por cada mes
    const char *nombresMeses[12] = { 
        "Enero", "Febrero", "Marzo", "Abril", "Mayo", 
        "Junio", "Julio", "Agosto", "Septiembre", 
        "Octubre", "Noviembre", "Diciembre"
    };

    // Itera sobre todas las ventas en la lista
    for (size_t i = 0; i < lista->tamano; i++) {
        int mes;
        sscanf(lista->ventas[i].fecha, "%*4d-%2d-%*2d", &mes); // Extrae el mes de la fecha
        if (mes >= 1 && mes <= 12) {
            ventasMensuales[mes - 1] += lista->ventas[i].total; // Acumula el total de ventas para el mes correspondiente
        } else {
            printf("Mes fuera de rango en la fecha: %s\n", lista->ventas[i].fecha);
        }
    }

    // Determina cuál mes tiene el mayor total de ventas
    int mesMayorVenta = 0;
    for (int i = 1; i < 12; i++) {
        if (ventasMensuales[i] > ventasMensuales[mesMayorVenta]) {
            mesMayorVenta = i;
        }
    }

    // Muestra el mes con el mayor total de ventas
    printf("El mes con mayor total de ventas es %s con un total de %.2f.\n", 
           nombresMeses[mesMayorVenta], ventasMensuales[mesMayorVenta]);
}

void diasSemanaMasActivo(const ListaVentas *lista) {
    int transaccionesPorDia[7] = {0}; // Array para contar transacciones por día de la semana
    const char *nombresDias[7] = { 
        "Domingo", "Lunes", "Martes", "Miércoles", 
        "Jueves", "Viernes", "Sábado" 
    };

    // Iterar sobre todas las ventas en la lista
    for (size_t i = 0; i < lista->tamano; i++) {
        struct tm tmFecha = {0};
        if (sscanf(lista->ventas[i].fecha, "%d-%d-%d", &tmFecha.tm_year, &tmFecha.tm_mon, &tmFecha.tm_mday) != 3) {
            printf("Error al analizar la fecha: %s\n", lista->ventas[i].fecha);
            continue;
        }
        tmFecha.tm_year -= 1900; // Ajustar el año
        tmFecha.tm_mon -= 1; // Ajustar el mes

        time_t tiempoFecha = mktime(&tmFecha); // Convierte la estructura tm a un tiempo calendar (time_t)
        if (tiempoFecha == -1) {
            printf("Error al convertir la fecha a time_t: %s\n", lista->ventas[i].fecha);
            continue;
        }

        struct tm *diaInfo = localtime(&tiempoFecha); // Obtiene la información del día
        if (diaInfo == NULL) {
            printf("Error al obtener la información del dia para la fecha: %s\n", lista->ventas[i].fecha);
            continue;
        }

        int diaSemana = diaInfo->tm_wday; // 0 = Domingo, 1 = Lunes, ..., 6 = Sábado
        transaccionesPorDia[diaSemana]++; // Incrementa el contador para el día correspondiente
    }

    // Determinar el día de la semana con más transacciones
    int diaMasActivo = 0;
    for (int i = 1; i < 7; i++) {
        if (transaccionesPorDia[i] > transaccionesPorDia[diaMasActivo]) {
            diaMasActivo = i;
        }
    }

    // Mostrar el día de la semana con más transacciones
    printf("El dia de la semana más activo es %s con un total de %d transacciones.\n", 
           nombresDias[diaMasActivo], transaccionesPorDia[diaMasActivo]);
}
