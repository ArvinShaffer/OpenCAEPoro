/*! \file    WellGroup.hpp
 *  \brief   WellGroup class declaration
 *  \author  Shizhe Li
 *  \date    Oct/01/2021
 *
 *-----------------------------------------------------------------------------------
 *  Copyright (C) 2021--present by the OpenCAEPoro team. All rights reserved.
 *  Released under the terms of the GNU Lesser General Public License 3.0 or later.
 *-----------------------------------------------------------------------------------
 */

#ifndef __WELLGROUP_HEADER__
#define __WELLGROUP_HEADER__

// OpenCAEPoro header files
#include "ParamWell.hpp"
#include "Well.hpp"

using namespace std;


/// WellGroups contains all wells now, it's used to manages all wells uniformly in
/// reservoirs. actually, you can regard it as an interface between wells and other
/// modules.
class WellGroup
{

    // temp
    friend class Reservoir;
public:
    WellGroup() = default;

    /////////////////////////////////////////////////////////////////////
    // General
    /////////////////////////////////////////////////////////////////////

public:
    /// Input param from ParamWell.
    void InputParam(const ParamWell& paramWell);
    /// Setup well in wellgroup.
    void Setup(const Grid& myGrid, const Bulk& myBulk);
    /// complete the information of well according to Grid and Bulk.
    void SetupWell(const Grid& myGrid, const Bulk& myBulk);
    /// get the mixture from bulk ---- usless now
    void SetupMixture(const Bulk& myBulk);
    /// Apply the operation mode at the ith critical time.
    void ApplyControl(const USI& i);
    /// Set the initial well pressure
    void InitBHP(const Bulk& myBulk);
    /// Calculate well properties at the beginning of each time step.
    void PrepareWell(const Bulk& myBulk);
    /// Calculate Transmissibility of Wells
    void CalTrans(const Bulk& myBulk);
    /// Calculate volume flow rate and moles flow rate of each perforation.
    void CalFlux(const Bulk& myBulk);
    /// Calculate dG.
    void CaldG(const Bulk& myBulk);
    /// Calculate Injection rate, total Injection, Production rate, total Production
    void CalIPRT(const Bulk& myBulk, OCP_DBL dt);
    /// Calculate memory for Matrix
    void AllocateMat(LinearSystem& myLS, const USI& bulknum) const;
    void UpdateLastBHP(){ for (auto& w : wellGroup) w.lBHP = w.BHP; }
    void ResetBHP();
    /// Reset dG to ldG for each well.
    void UpdateLastDg() { for (auto& w : wellGroup) w.ldG = w.dG; }
    void ResetDg() { for (auto& w : wellGroup) w.dG = w.ldG; }
    /// Check if unreasonable well pressure or perforation pressure occurs.
    OCP_INT CheckP(const Bulk& myBulk);
    /// Return the num of wells.
    USI GetWellNum() const { return numWell; }
    /// Return the name of specified well.
    string GetWellName(const USI& i) const { return wellGroup[i].name; }
    /// Return the index of specified well.
    USI GetIndex(const string& name) const;
    /// Return the num of perforations of well i.
    USI GetWellPerfNum(const USI& i)const { return wellGroup[i].numPerf; }
    /// Calculate mamimum num of perforations of all Wells.
    USI GetMaxWellPerNum() const;
    void CalMaxBHPChange();
    OCP_DBL GetdBHPmax()const { return dPmax; }
    /// Return oil production rate in field.
    OCP_DBL GetFOPR() const { return FOPR; }
    /// Return total oil production in field.
    OCP_DBL GetFOPT() const { return FOPT; }
    /// Return gas production rate in field.
    OCP_DBL GetFGPR() const { return FGPR; }
    /// Return total gas production in field.
    OCP_DBL GetFGPT() const { return FGPt; }
    /// Return water production rate in field.
    OCP_DBL GetFWPR() const { return FWPR; }
    /// Return total water production in field.
    OCP_DBL GetFWPT() const { return FWPT; }
    /// Return gas injection rate in field.
    OCP_DBL GetFGIR() const { return FGIR; }
    /// Return gas water injection in field.
    OCP_DBL GetFGIT() const { return FGIT; }
    /// Return water injection rate in field.
    OCP_DBL GetFWIR() const { return FWIR; }
    /// Return total water injection in field.
    OCP_DBL GetFWIT() const { return FWIT; }
    /// Return oil production rate of the wth well.
    OCP_DBL GetWOPR(const USI& w) const { return wellGroup[w].WOPR; }
    /// Return total oil production of the wth well.
    OCP_DBL GetWOPT(const USI& w) const { return wellGroup[w].WOPT; }
    /// Return gas production rate of the wth well.
    OCP_DBL GetWGPR(const USI& w) const { return wellGroup[w].WGPR; }
    /// Return total gas production of the wth well.
    OCP_DBL GetWGPT(const USI& w) const { return wellGroup[w].WGPT; }
    /// Return water production rate of the wth well.
    OCP_DBL GetWWPR(const USI& w) const { return wellGroup[w].WWPR; }
    /// Return total water production of the wth well.
    OCP_DBL GetWWPT(const USI& w) const { return wellGroup[w].WWPT; }
    /// Return gas injection rate of the wth well.
    OCP_DBL GetWGIR(const USI& w) const { return wellGroup[w].WGIR; }
    /// Return total gas injection of the wth well.
    OCP_DBL GetWGIT(const USI& w) const { return wellGroup[w].WGIT; }
    /// Return water injection rate of the wth well.
    OCP_DBL GetWWIR(const USI& w) const { return wellGroup[w].WWIR; }
    /// Return total water injection of the wth well.
    OCP_DBL GetWWIT(const USI& w) const { return wellGroup[w].WWIT; }
    /// Return the BHP of wth well.
    OCP_DBL GetWBHP(const USI& w) const { return wellGroup[w].BHP; }
    /// Return the pth dG of wth well.
    OCP_DBL GetWellDg(const USI& w, const USI& p)const { return wellGroup[w].dG[p]; }

private:
    USI          numWell;   ///< num of wells.
    vector<Well> wellGroup; ///< well set.

    OCP_DBL      dPmax{ 0 };     ///< Maximum BHP change

    vector<Mixture*> flashCal; ///< Uesless now.

    OCP_DBL FGIR{ 0 }; ///< gas injection rate in field.
    OCP_DBL FGIT{ 0 }; ///< gas total injection in field.
    OCP_DBL FWIR{ 0 }; ///< water injection rate in field.
    OCP_DBL FWIT{ 0 }; ///< water total injection in field.
    OCP_DBL FOPR{ 0 }; ///< oil production rate in field.
    OCP_DBL FOPT{ 0 }; ///< oil total production in field.
    OCP_DBL FGPR{ 0 }; ///< gas production rate in field.
    OCP_DBL FGPt{ 0 }; ///< gas total production in field.
    OCP_DBL FWPR{ 0 }; ///< water production rate in field.
    OCP_DBL FWPT{ 0 }; ///< water total production in field.


    /////////////////////////////////////////////////////////////////////
    // IMPEC
    /////////////////////////////////////////////////////////////////////

public:
    /// Calculate the CFL number for each perforation and return the maximum one.
    OCP_DBL CalCFLIMPEC(const Bulk& myBulk, const OCP_DBL& dt) const;
    /// Calculate the CFL number for each perforation and return the maximum one.
    void CalCFL01IMPEC(const Bulk& myBulk, const OCP_DBL& dt) const;
    /// Update moles of components in Bulks which connects to well
    void MassConserveIMPEC(Bulk& myBulk, OCP_DBL dt);
    /// Assemble matrix, parts related to well are included for IMPEC
    void AssemblaMatIMPEC(LinearSystem& myLS, const Bulk& myBulk,
            const OCP_DBL& dt) const;
    /// Get solution from solver class after linear system is solved for IMPEC
    void GetSolIMPEC(const vector<OCP_DBL>& u, const OCP_USI& bId);


    /////////////////////////////////////////////////////////////////////
    // FIM
    /////////////////////////////////////////////////////////////////////

public:
    /// Assemble matrix, parts related to well are included for FIM
    void AssemblaMatFIM(LinearSystem& myLS, const Bulk& myBulk,
        const OCP_DBL& dt) const;
    /// Get solution from solver class after linear system is solved for IMPEC for FIM
    void GetSolFIM(const vector<OCP_DBL>& u, const OCP_USI& bId, const USI& len); 
    void GetSol01FIM(const vector<OCP_DBL>& u, const OCP_USI& bId, const USI& len, const OCP_DBL& alpha);
    /// Calculate Resiual and relative Resiual for FIM
    void CalResFIM(ResFIM& resFIM, const Bulk& myBulk, const OCP_DBL& dt) const;

};


#endif /* end if __WELLGROUP_HEADER__ */

/*----------------------------------------------------------------------------*/
/*  Brief Change History of This File                                         */
/*----------------------------------------------------------------------------*/
/*  Author              Date             Actions                              */
/*----------------------------------------------------------------------------*/
/*  Shizhe Li           Oct/01/2021      Create file                          */
/*  Chensong Zhang      Oct/15/2021      Format file                          */
/*----------------------------------------------------------------------------*/