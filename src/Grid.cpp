/*! \file    Grid.cpp
 *  \brief   Grid class definition
 *  \author  Shizhe Li
 *  \date    Oct/01/2021
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2021--present by the OpenCAEPoro team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#include "Grid.hpp"

void Grid::InputParam(const ParamReservoir& rs_param)
{
    nx = rs_param.dimens.nx;
    ny = rs_param.dimens.ny;
    nz = rs_param.dimens.nz;
    numGrid = rs_param.numGrid;

    if (!rs_param.coord.empty()) {
        // CornerPoint Grid
        gridType = CORNER_GRID;

        coord = rs_param.coord;
        zcorn = rs_param.zcorn;
    }
    else {
        // Orthogonal Grid
        gridType = ORTHOGONAL_GRID;

        numConn = 3 * nx * ny * nz - nx * ny - ny * nz - nz * nx;
        tops = rs_param.tops;
        dx = rs_param.dx;
        dy = rs_param.dy;
        dz = rs_param.dz;
    }


    ntg = rs_param.ntg;
    poro = rs_param.poro;
    kx = rs_param.permX;
    ky = rs_param.permY;
    kz = rs_param.permZ;

    SATNUM.resize(numGrid, 0);
    if (rs_param.SATNUM.activity) {
        for (OCP_USI i = 0; i < numGrid; i++) {
            SATNUM[i] = (USI)(rs_param.SATNUM.data[i]) - 1;
        }
    }
    PVTNUM.resize(numGrid, 0);
    if (rs_param.PVTNUM.activity) {
        for (OCP_USI i = 0; i < numGrid; i++) {
            PVTNUM[i] = (USI)(rs_param.PVTNUM.data[i]) - 1;
        }
    }
    cout << "Grid::InputParam" << endl;
}

void Grid::Setup()
{
    switch (gridType)
    {
    case ORTHOGONAL_GRID:
        SetupOthogonalGrid();
        break;
    case CORNER_GRID:
        SetupCornerGrid();
        break;
    default:
        OCP_ABORT("WRONG Grid Type!");
        break;
    }
}



void Grid::SetupOthogonalGrid()
{
    // x -> y -> z
    SetupNeighborOthogonalGrid();
    CalDepthVOthogonalGrid();
    CalActiveGrid(1E-6, 1E-6);
}

void Grid::SetupNeighborOthogonalGrid()
{

    gNeighbor.resize(numGrid);
    // PreAllocate
    for (OCP_USI n = 0; n < numGrid; n++) {
        gNeighbor[n].reserve(6);
    }

    // Begin Id and End Id in Grid, bIdg > eIdg
    OCP_USI bIdg, eIdg;
    OCP_DBL area;
    OCP_USI nxny = nx * ny;

    for (USI k = 0; k < nz; k++) {
        for (USI j = 0; j < ny; j++) {
            for (USI i = 0; i < nx; i++) {

                bIdg = k * nxny + j * nx + i;
                // right  --  x-direction
                if (i < nx - 1) {
                    eIdg = bIdg + 1;
                    area = CalAkdOthogonalGrid(bIdg, eIdg, 1);
                    gNeighbor[bIdg].push_back(GPair(eIdg, area));
                    gNeighbor[eIdg].push_back(GPair(bIdg, area));
                }

                // front  --  y-direction
                if (j < ny - 1) {
                    eIdg = bIdg + nx;
                    area = CalAkdOthogonalGrid(bIdg, eIdg, 2);
                    gNeighbor[bIdg].push_back(GPair(eIdg, area));
                    gNeighbor[eIdg].push_back(GPair(bIdg, area));
                }

                // down --   z-direction
                if (k < nz - 1) {
                    eIdg = bIdg + nxny;
                    area = CalAkdOthogonalGrid(bIdg, eIdg, 3);
                    gNeighbor[bIdg].push_back(GPair(eIdg, area));
                    gNeighbor[eIdg].push_back(GPair(bIdg, area));
                }
            }
        }
    }
    OCP_FUNCNAME;
}

OCP_DBL Grid::CalAkdOthogonalGrid(const OCP_USI& bId, const OCP_USI& eId, const USI& direction)
{
    OCP_DBL T1;
    OCP_DBL T2;
    switch (direction)
    {
    case 1:
        // x-direction
        T1 = kx[bId] * ntg[bId] * dy[bId] * dz[bId] / dx[bId];
        T2 = kx[eId] * ntg[eId] * dy[eId] * dz[eId] / dx[eId];
        return (2 / (1 / T1 + 1 / T2));
        break;
    case 2:
        // y-direction
        T1 = ky[bId] * ntg[bId] * dz[bId] * dx[bId] / dy[bId];
        T2 = ky[eId] * ntg[eId] * dz[eId] * dx[eId] / dy[eId];
        return (2 / (1 / T1 + 1 / T2));
        break;
    case 3:
        // z-direction -- no ntg
        T1 = kz[bId] * dx[bId] * dy[bId] / dz[bId];
        T2 = kz[eId] * dx[eId] * dy[eId] / dz[eId];
        return (2 / (1 / T1 + 1 / T2));
        break;
    default:
        OCP_ABORT("Wrong Direction!");
        break;
    }

}

void Grid::CalDepthVOthogonalGrid()
{
    depth.resize(numGrid, 0);
    OCP_USI nxny = nx * ny;
    // 0th layer
    for (USI j = 0; j < ny; j++) {
        for (USI i = 0; i < nx; i++) {
            OCP_USI id = j * nx + i;
            depth[id]  = tops[id] + dz[id] / 2;
        }
    }
    // 1th - (nz-1)th layer
    for (USI k = 1; k < nz; k++) {
        OCP_USI knxny = k * nxny;
        for (USI j = 0; j < ny; j++) {
            for (USI i = 0; i < nx; i++) {
                OCP_USI id = knxny + j * nx + i;
                depth[id]  = depth[id - nxny] + dz[id - nxny] / 2 + dz[id] / 2;
            }
        }
    }

    v.resize(numGrid);
    for (OCP_USI i = 0; i < numGrid; i++) v[i] = dx[i] * dy[i] * dz[i];
    cout << "Grid::calDepthV" << endl;
}

void Grid::SetupCornerGrid()
{
    COORD coordTmp;
    coordTmp.Allocate(nx, ny, nz);
    coordTmp.InputData(coord, zcorn);
    coordTmp.CalConn();
    SetupNeighborCornerGrid(coordTmp);
    CalActiveGrid(1E-6, 1E-6);
}

void Grid::SetupNeighborCornerGrid(const COORD& CoTmp)
{

    dx = CoTmp.dx;
    dy = CoTmp.dy;
    dz = CoTmp.dz;
    v = CoTmp.v;
    depth = CoTmp.depth;

    gNeighbor.resize(numGrid);
    // PreAllocate
    for (OCP_USI n = 0; n < numGrid; n++) {
        gNeighbor[n].reserve(10);
    }

    OCP_USI bIdg, eIdg;
    OCP_DBL area;

    for (OCP_USI n = 0; n < CoTmp.numConn; n++) {
        const GeneralConnect& ConnTmp = CoTmp.connect[n];
        bIdg = ConnTmp.begin;
        eIdg = ConnTmp.end;
        area = CalAkdCornerGrid(ConnTmp);
        gNeighbor[bIdg].push_back(GPair(eIdg, area));
        gNeighbor[eIdg].push_back(GPair(bIdg, area));
    }

    cout << "Done" << endl;
}

OCP_DBL Grid::CalAkdCornerGrid(const GeneralConnect& conn)
{
    OCP_USI bId = conn.begin;
    OCP_USI eId = conn.end;
    OCP_DBL bArea = conn.Ad_dd_begin;
    OCP_DBL eArea = conn.Ad_dd_end;
    OCP_DBL T1, T2;

    switch (conn.directiontype)
    {
    case 1:
        T1 = ntg[bId] * kx[bId] * bArea;
        T2 = ntg[eId] * kx[eId] * eArea;
        return 1 / (1 / T1 + 1 / T2);
        break;
    case 2:
        T1 = ntg[bId] * ky[bId] * bArea;
        T2 = ntg[eId] * ky[eId] * eArea;
        return 1 / (1 / T1 + 1 / T2);
        break;
    case 3:
        T1 = kz[bId] * bArea;
        T2 = kz[eId] * eArea;
        return 1 / (1 / T1 + 1 / T2);
        break;
    default:
        OCP_ABORT("Wrong Direction Type!");
        break;
    }
}


void Grid::CalActiveGrid(const OCP_DBL& e1, const OCP_DBL& e2)
{
    activeMap_B2G.reserve(numGrid);
    activeMap_G2B.resize(numGrid);
    OCP_USI count = 0;
    for (OCP_USI n = 0; n < numGrid; n++) {
        if (poro[n] * ntg[n] < e1 || dx[n] * dy[n] * dz[n] < e2) {
            activeMap_G2B[n] = GB_Pair(false, 0);
            continue;
        }
        activeMap_B2G.push_back(n);
        activeMap_G2B[n] = GB_Pair(true, count);
        count++;
    }
    activeGridNum = count;
    cout << (numGrid - activeGridNum)*100.0 / numGrid << "% (" << (numGrid - activeGridNum) << ") of grid cell is inactive" << endl;
    cout << "Grid::calActiveBulk" << endl;
}



OCP_USI Grid::GetActIndex(const USI& i, const USI& j, const USI& k) const
{
    OCP_USI id       = k * nx * ny + j * nx + i;
    bool    activity = activeMap_G2B[id].IsAct();
    if (!activity) {
        OCP_ABORT("(" + to_string(i) + "," + to_string(j) + "," + to_string(k) +
                   ") is inactive");
    }
    id = activeMap_G2B[id].GetId();
    return id;
}

/*----------------------------------------------------------------------------*/
/*  Brief Change History of This File                                         */
/*----------------------------------------------------------------------------*/
/*  Author              Date             Actions                              */
/*----------------------------------------------------------------------------*/
/*  Shizhe Li           Oct/01/2021      Create file                          */
/*  Chensong Zhang      Oct/15/2021      Format file                          */
/*----------------------------------------------------------------------------*/