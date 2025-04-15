#ifndef _BMS_HPP_
#define _BMS_HPP_

/* ---------- Includes section ---------- */
#include <iostream>
#include <string.h>
#include <array>
#include <stdio.h>
#include <math.h>

/* ---------- Macros section ---------- */
#define NUM_CELLS           2
#define MAX_VOLTAGE         4.2
#define MIN_VOLTAGE         3.0
#define MAX_TEMPRATURE      60.0
#define MIN_TEMPRATURE      0.0
#define BALANCE_THRESHOLD   0.05
#define NOT_BALANCED        0
#define BALANCED            1

/* ---------- types section ---------- */
typedef enum {
    OK,
    UNDER_VOLTAGE,
    OVER_VOLTAGE,
    UNDER_TEMPRATURE,
    OVER_TEMPRATURE,
}CellStatus_t;

typedef struct {
    float voltage;
    float temprature;
    float statusOfCharge;
    CellStatus_t status;
}Cell_t;

/* ---------- API section ---------- */
void initCells(const std::array<Cell_t, NUM_CELLS> &copyCell);

#endif