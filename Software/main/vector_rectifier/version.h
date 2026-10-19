/***********************************************************************
 * File:        version.h
 * Device:      TMS320F28335
 * Author:      Luciano Raffagnini (lraffagnini@outlook.com)
 * Description: Archivo para hacer seguimiento de los cambios en el
 *              proyecto.
 *              Si, ya se, ya se, un poco tarde lo hice pero mas vale
 *              tarde que nunca. Espero que el siguiente grupo le de
 *              un mejor uso que yo.
 *              En fin, la idea es usar:
 *
 *              VERSION_MAYOR para indicar cambios muy importantes,
 *              como cambios en la estructura de control o la logica
 *              de funcionamiento.
 *
 *              VERSION_MINOR para cuando se agrega algun funcionamiento
 *              extra de menor magnitud
 *
 *              VERSION_PATCH si se resuelve un bug o se hace un fix
 *
 * History:
 *   24/09/2026 - v 4.0.0 original: Terrible arrancar con una v4, sean
 *                mas prolijos. Heredo el 4 del proyecto original
 *                "rectificador_vectorial_rev4"
 *
 *   24/09/2026 - v 4.1.0: Se implementa el apagado controlado con
 *                descarga sobre la carga.
 * 
 *   16/10/2026 - v 5.0.0: Se emprolija ttodo el sistema de apis, se 
 *                renombran archivos para mayor coherencia. Todo lo 
 *                relacionado con texas quedo en la subcarpeta "texas", 
 *                los metodos de alto nivel quedaron ligados a "project"
 *                y los metodos de mas bajo nivel estan en "rectifier"
 **********************************************************************/

#ifndef VERSION_H
#define VERSION_H

#define VERSION_MAYOR 5
#define VERSION_MINOR 0
#define VERSION_PATCH 0

#endif
