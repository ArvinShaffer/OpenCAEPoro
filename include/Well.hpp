/*! \file    Well.hpp
 *  \brief   Well class declaration
 *  \author  Shizhe Li
 *  \date    Oct/01/2021
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2021--present by the OpenCAEPoro team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#ifndef __WELL_HEADER__
#define __WELL_HEADER__

// Standard header files
#include <cassert>

// OpenCAEPoro header files
#include "Bulk.hpp"
#include "DenseMat.hpp"
#include "Grid.hpp"
#include "LinearSystem.hpp"
#include "OCPConst.hpp"
#include "OCPStructure.hpp"
#include "ParamWell.hpp"
#include "WellPerf.hpp"

using namespace std;

/// WellOpt describes the operation mode of a well.
/// usually it changes over time, specifically, each attributes could be changed
/// including the well type.
class WellOpt
{
    friend class Well;

public:
    /// Default constructor.
    WellOpt() = default;

    /// Constructor well operation mode using params.
    WellOpt(const WellOptParam& Optparam);

private:
    USI type{0}; ///< type of well, Inj or Prod.
    /// indicate which type of fluids will be injected, water, gas, or other solvent.
    /// it's decided by users and only useful for injection well.
    USI  fluidType{0};
    bool state{false}; ///< state of well, close or open.
    USI  optMode; ///< control mode of well: constant pressure, or constant flow rate of
                  ///< specified fluids.
    /// it gives the upper limit of flow rate of specified fluids if the well is under
    /// the control of constant pressure. it gives the flow rate of specified fluids if
    /// the well is under the control of constant flow rate.
    OCP_DBL maxRate;
    /// used for injection well.
    /// it gives the upper limit of well pressure if the well is under the control of
    /// constant flow rate. it gives the pressure of well if the well is under the
    /// control of constant pressure.
    OCP_DBL maxBHP;
    /// used for production well.
    /// it gives the lower limit of well pressure if the well is under the control of
    /// constant flow rate. it gives the pressure of well if the well is under the
    /// control of constant pressure.
    OCP_DBL minBHP;
    /// it's decided by users input.
    /// for injection well, it describes the components of injected fluids.
    /// for production well, it gives the the components of fluids which we are
    /// interested in.
    vector<OCP_DBL> zi;
};

/// Well class defines well, and any operations referred to wells are in it.
/// Well connects to the bulks by perforations, which serve as source and sink.
/// Due to practical difficulties in production, a good treatment for well is important,
/// excellent treatment will make the flow rate in well more stable.
class Well
{
    friend class WellGroup;

public:
    Well() = default;

    /////////////////////////////////////////////////////////////////////
    // General
    /////////////////////////////////////////////////////////////////////

public:
    /// Input the param of perforations.
    void InputPerfo(const WellParam& well);
    /// Setup the well after Grid and Bulk finish setupping.
    void Setup(const Grid& myGrid, const Bulk& myBulk);
    /// Initialize the Well BHP
    void InitBHP(const Bulk& myBulk);
    /// Calculate Well Index with Peaceman model for vertical well.
    void CalWI_Peaceman_Vertical(const Bulk& myBulk);
    /// Calculate transmissibility for each phase in perforations.
    void CalTrans(const Bulk& myBulk);
    /// Calculate the flux for each perforations.
    void CalFlux(const Bulk& myBulk, const bool flag = false);
    /// calculate flow rate of moles of components for injection well in black oil model
    OCP_DBL CalInjRateBO(const Bulk& myBulk);
    /// calculate flow rate of moles of components for production well in black oil model
    OCP_DBL CalProdRateBO(const Bulk& myBulk);
    /// Calculate flow rate of moles of components for injection well in black oil model.
    void CalInjQiBO(const Bulk& myBulk, const OCP_DBL& dt);
    /// Calculate flow rate of moles of components for Production well in black oil model.
    void CalProdQiBO(const Bulk& myBulk, const OCP_DBL& dt);
    /// Calculate pressure difference between well and perforations.
    void CaldG(const Bulk& myBulk);
    /// Calculate pressure difference between well and perforations for Injection.
    void CalInjdG(const Bulk& myBulk);
    /// Calculate pressure difference between well and perforations for Prodcution.
    void CalProddG(const Bulk& myBulk);
    /// Set BHP if opt mode is BHPMode
    void SetBHP();
    /// Try to smooth the dG by average it with dG at last time step.
    void SmoothdG();
    /// Check if well operation mode would be changed.
    void CheckOptMode(const Bulk& myBulk);
    /// Check if abnormal Pressure occurs.
    OCP_INT CheckP(const Bulk& myBulk);
    /// Check if crossflow happens.
    OCP_INT CheckCrossFlow(const Bulk& myBulk);
    /// Update pressure in Perforation after well pressure updates.
    void UpdatePerfP() { for (USI p = 0; p < numPerf; p++) perf[p].P = BHP + dG[p]; }
    /// Allocate memory for matrix.
    void AllocateMat(LinearSystem& myLS) const;
    /// Return the state of the well, Open or Close.
    bool WellState() const { return opt.state; }
    /// Return the type of well, Inj or Prod.
    USI WellType() const { return opt.type; } 
    /// Return Pressure of Perf p
    OCP_DBL GetPerfPre(const USI& p)const { return perf[p].P; }
    /// Display operation mode of well and state of perforations.
    void ShowPerfStatus() const;

private:
    string  name; ///< well name
    USI     I;    ///< I-index of the well header.
    USI     J;    ///< J-index of the well header.
    WellOpt opt;  ///< well control parameters, contains current control parameters.
    vector<WellOpt> optSet;      ///< well control parameters set, contains control
                                 ///< parameters in all critical time.
    OCP_DBL             lBHP;    ///< Last well pressure in reference depth.
    mutable OCP_DBL     BHP;     ///< well pressure in reference depth.
    OCP_DBL             depth;   ///< reference depth of well.
    USI                 numPerf; ///< num of perforations belonging to this well.
    vector<Perforation> perf;    ///< information of perforation belonging to this well.
    vector<OCP_DBL>
                    dG; ///< difference of pressure between well and perforation: numPerf.
    vector<OCP_DBL> ldG; ///< difference of pressure between well and perforation at
                         ///< last time step: numPerf.

    // production rate and injection rate
    vector<OCP_DBL> qi_lbmol; ///< flow rate of moles of component inflowing/outflowing
                              ///< well: num of components.
    OCP_DBL WOPR{0};          ///< well oil production rate.
    OCP_DBL WOPT{0};          ///< well total oil production.
    OCP_DBL WGPR{0};          ///< well gas production rate.
    OCP_DBL WGPT{0};          ///< well total gas production.
    OCP_DBL WWPR{0};          ///< well water production rate.
    OCP_DBL WWPT{0};          ///< well total water production.
    OCP_DBL WGIR{0};          ///< well gas injection rate.
    OCP_DBL WGIT{0};          ///< well total gas injection.
    OCP_DBL WWIR{0};          ///< well water injection rate.
    OCP_DBL WWIT{0};          ///< well total water injection.



    /////////////////////////////////////////////////////////////////////
    // IMPEC
    /////////////////////////////////////////////////////////////////////

public:
    /// Calculate the CFL number, only parts related to wells are considered.
    OCP_DBL CalCFLIMPEC(const Bulk& myBulk, const OCP_DBL& dt) const;
    /// Calculate the CFL number, only parts related to wells are considered.
    void CalCFL01IMPEC(const Bulk& myBulk, const OCP_DBL& dt) const;
    /// Update moles of components in those bulks who connects to the well.
    void MassConserveIMPEC(Bulk& myBulk, const OCP_DBL& dt) const;
    /// Assemble matrix for IMPEC, parts related to injection well are included.
    void AssembleMatINJ_IMPEC(const Bulk& myBulk, LinearSystem& myLS,
        const OCP_DBL& dt) const;
    /// Assemble matrix for IMPEC, parts related to production well are included.
    void AssembleMatPROD_BO_IMPEC(const Bulk& myBulk, LinearSystem& myLS,
        const OCP_DBL& dt) const;



    /////////////////////////////////////////////////////////////////////
    // FIM
    /////////////////////////////////////////////////////////////////////

public:
    /// Assemble matrix for FIM, parts related to Injection well are included.
    void AssembleMatINJ_FIM(const Bulk& myBulk, LinearSystem& myLS,
        const OCP_DBL& dt) const;
    /// Assemble matrix for FIM, parts related to Production well are included.
    void AssembleMatPROD_BO_FIM(const Bulk& myBulk, LinearSystem& myLS,
        const OCP_DBL& dt) const;
    /// Calculate Resiual and relative Resiual for FIM.
    void CalResFIM(ResFIM& resFIM, const Bulk& myBulk, const OCP_DBL& dt,
        const OCP_USI& wId) const;

};

#endif /* end if __WELL_HEADER__ */

/*----------------------------------------------------------------------------*/
/*  Brief Change History of This File                                         */
/*----------------------------------------------------------------------------*/
/*  Author              Date             Actions                              */
/*----------------------------------------------------------------------------*/
/*  Shizhe Li           Oct/01/2021      Create file                          */
/*  Chensong Zhang      Oct/15/2021      Format file                          */
/*----------------------------------------------------------------------------*/