/**
 * @file facturacodigo.c
 * @author Emilio J y Wagner B. 
 * @brief Este programa permite genera una factura, solicitando los datos del cliente, tomando la fecha del sistema, solicitando los productos, 
 *        la tasa del IVA para la factura, calcula el precio total, precio subtotal, el iva, y presenta, 
 *        además también guarda el resultado en un archivo ".txt"
 * @version 0.1
 * @date 2024-02-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    char codigo[10];
    float cantidad;
    char descripcion[100];
    float precioUnitario;
} Producto;

typedef struct{
    char nombres[100];
    char cedula[20];
    char correo[100];
    char direccion[100];
    int id;
} Cliente;


int generarNumeroFactura();
void obtenerFecha(char *fecha);
void solicitarCliente(Cliente *cliente);
float calcularPrecioTotal(float cantidad, float precioUnitario);
void ingresarProductos(Producto productos[], int *numProductos);
float solicitarTasaIVA();
float calcularSubtotal(Producto productos[], int numProductos);
float calcularIva(float subtotal, float tasaIVA);
void presentarFacturaTerminal(Cliente cliente, Producto productos[], int numProductos, float subtotal, float iva, float total);
void presentarFacturaArchivo(FILE *archivo, Cliente cliente, Producto productos[], int numProductos, float subtotal, float iva, float total);


int main() {
    srand(time(NULL));

    Cliente cliente;
    solicitarCliente(&cliente);
    Producto productos[10];
    int numProductos;
    ingresarProductos(productos, &numProductos);
    float tasaIVA = solicitarTasaIVA();
    float subtotal = calcularSubtotal(productos, numProductos);
    float iva = calcularIva(subtotal, tasaIVA);
    float total = subtotal + iva;
    system ("@cls||clear");
    presentarFacturaTerminal(cliente, productos, numProductos, subtotal, iva, total);
    char conf;
    while (conf != 'y' && conf != 'n')    {
        printf("Desea aceptar su compra (y/n):");
        scanf("%c", &conf);
        if (conf != 'y' && conf != 'n')
        {
            printf("La respuesta no es correcta.");
        }
    }     
    if (conf == 'y' ) {
            FILE *archivo;
            archivo = fopen("factura.txt", "w");
            if (archivo == NULL) {
                printf("Error al abrir el archivo.");
                return 0;
            }
            presentarFacturaArchivo(archivo, cliente, productos, numProductos, subtotal, iva, total);

            fclose(archivo);

            printf("La factura se ha guardado correctamente.\n");
            printf("Archivo: factura.txt\n");
            system("pause");
    } else {
        printf ("Compra Rechazada.\n");
        system ("pause");
    }
    return 0;
}

int generarNumeroFactura() {
    int numero = rand() % 1000000 + 1;
    return numero;
}

void obtenerFecha(char *fecha) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(fecha, 20, "%Y-%m-%d", tm);
}

float calcularPrecioTotal(float cantidad, float precioUnitario) {
    float total = cantidad * precioUnitario;
    return total;
}

void solicitarCliente(Cliente *cliente) {
    
    printf("Ingrese el nombre del cliente: ");
    fgets(cliente->nombres, sizeof(cliente->nombres), stdin);

    printf("Ingrese la cedula del cliente: ");
    fgets(cliente->cedula, sizeof(cliente->cedula), stdin);

    printf("Ingrese el correo del cliente: ");
    fgets(cliente->correo, sizeof(cliente->correo), stdin);

    printf("Ingrese la direccion del cliente: ");
    fgets(cliente->direccion, sizeof(cliente->direccion), stdin);

    cliente->id = rand() % 1000000 + 1;
}

void ingresarProductos(Producto productos[], int *numProductos) {
    printf("Ingrese el numero de productos comprados: ");
    scanf("%d", numProductos);
    getchar();

    printf("Ingrese los detalles de cada producto:\n");
    for (int i = 0; i < *numProductos; i++) {
        printf("\n");
        printf("Producto %d:\n", i + 1);
        printf("Codigo: ");
        scanf("%s", productos[i].codigo);
        printf("Cantidad: ");
        scanf("%f", &productos[i].cantidad);
        printf("Descripcion: ");
        scanf("%s", productos[i].descripcion);
        printf("Precio Unitario: ");
        scanf("%f", &productos[i].precioUnitario);
        getchar(); 
    }
}

float solicitarTasaIVA() {
    float tasaIVA;
    printf("\n");
    printf("Ingrese la tasa del IVA en porcentaje: ");
    scanf("%f", &tasaIVA);
    getchar();
    return tasaIVA;
}

float calcularSubtotal(Producto productos[], int numProductos) {
    float subtotal = 0;
    for (int i = 0; i < numProductos; i++) {
        subtotal += calcularPrecioTotal(productos[i].cantidad, productos[i].precioUnitario);
    }
    return subtotal;
}

float calcularIva(float subtotal, float tasaIVA) {
    float Iva = subtotal * (tasaIVA / 100); 
    return Iva;
}

void presentarFacturaTerminal(Cliente cliente, Producto productos[], int numProductos, float subtotal, float iva, float total) {
    char fecha[20];
    obtenerFecha(fecha);
    
    printf("\n");
    printf("------------------------------------------------------------\n");
    printf("                      FACTURA DE VENTA\n");
    printf(" EMPRESA BALJAR\n");
    printf(" ----------------------------------------------------------\n");
    printf(" Fecha de emision:                              %s\n", fecha);
    printf(" Nro. factura:                                         %d\n", generarNumeroFactura());
    printf(" ----------------------------------------------------------\n");
    printf(" Cliente\n");
    printf(" ----------------------------------------------------------\n");
    printf(" Nombres:   %s", cliente.nombres);
    printf(" Cedula:    %s", cliente.cedula);
    printf(" Correo:    %s", cliente.correo);
    printf(" Direccion: %s", cliente.direccion);
    printf(" Id:        %d\n", cliente.id);
    printf(" ----------------------------------------------------------\n");
    printf(" LISTADO DE PRODUCTOS\n");
    printf(" ----------------------------------------------------------\n");
    printf(" Codigo   Cantidad   Descripcion          Precio U.   Total\n");
    for (int i = 0; i < numProductos; i++) {
        printf(" %-8s %-10.2f %-20s %-10.2f %-6.2f\n", productos[i].codigo, productos[i].cantidad, productos[i].descripcion, productos[i].precioUnitario, calcularPrecioTotal(productos[i].cantidad, productos[i].precioUnitario));
    }
    printf(" ----------------------------------------------------------\n");
    printf(" Subtotal Neto: %.2f USD\n", subtotal);
    printf(" IVA(%.2f%%): %.2f USD\n", (iva / subtotal) * 100, iva);
    printf(" Total a pagar : %.2f USD\n", total);
    printf(" ----------------------------------------------------------\n");
}

void presentarFacturaArchivo(FILE *archivo, Cliente cliente, Producto productos[], int numProductos, float subtotal, float iva, float total) {
    char fecha[20];
    obtenerFecha(fecha);

    fprintf(archivo, "\n");
    fprintf(archivo, "------------------------------------------------------------\n");
    fprintf(archivo, "                      FACTURA DE VENTA\n");
    fprintf(archivo, " EMPRESA BARJAR\n");
    fprintf(archivo, " ----------------------------------------------------------\n");
    fprintf(archivo, " Fecha de emision:                                %s\n", fecha);
    fprintf(archivo, " Nro. factura:                                        %d\n", generarNumeroFactura());
    fprintf(archivo, " ----------------------------------------------------------\n");
    fprintf(archivo, " Cliente\n");
    fprintf(archivo, " ----------------------------------------------------------\n");
    fprintf(archivo, " Nombres:   %s", cliente.nombres);
    fprintf(archivo, " Cedula:    %s", cliente.cedula);
    fprintf(archivo, " Correo:    %s", cliente.correo);
    fprintf(archivo, " Direccion: %s", cliente.direccion);
    fprintf(archivo, " Id:        %d\n", cliente.id);
    fprintf(archivo, " ----------------------------------------------------------\n");
    fprintf(archivo, " LISTADO DE PRODUCTOS\n");
    fprintf(archivo, " ----------------------------------------------------------\n");
    fprintf(archivo, " Codigo   Cantidad   Descripcion          Precio U.   Total\n");
    for (int i = 0; i < numProductos; i++) {
        fprintf(archivo, " %-8s %-10.2f %-20s %-10.2f %-6.2f\n", productos[i].codigo, productos[i].cantidad, productos[i].descripcion, productos[i].precioUnitario, calcularPrecioTotal(productos[i].cantidad, productos[i].precioUnitario));
    }
    fprintf(archivo, " ----------------------------------------------------------\n");
    fprintf(archivo, " Subtotal Neto: %.2f USD\n", subtotal);
    fprintf(archivo, " IVA(%.2f%%): %.2f USD\n", (iva / subtotal) * 100, iva);
    fprintf(archivo, " Total a pagar : %.2f USD\n", total);
    fprintf(archivo, " ----------------------------------------------------------\n");
    fprintf(archivo, "         !!!!!!!GRACIAS POR TU COMPRA!!!!!!!\n");
    fprintf(archivo, " ----------------------------------------------------------\n");
    fprintf(archivo, "Programado por: Emilio Jaramillo y Wagner Balcazar\n");
    fprintf(archivo, "============================================================\n");
}