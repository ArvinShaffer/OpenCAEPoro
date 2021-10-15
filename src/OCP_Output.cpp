#include "OCP_Output.hpp"

void Summary::InputParam(const OutputSummary& summary_param)
{
	FPR = summary_param.FPR;
	FOPR = summary_param.FOPR;
	FOPT = summary_param.FOPT;
	FGPR = summary_param.FGPR;
	FGPt = summary_param.FGPt;
	FWPR = summary_param.FWPR;
	FWPT = summary_param.FWPT;
	FGIR = summary_param.FGIR;
	FGIT = summary_param.FGIT;
	FWIR = summary_param.FWIR;
	FWIT = summary_param.FWIT;


	WOPR = summary_param.WOPR;
	WOPT = summary_param.WOPT;
	WGPR = summary_param.WGPR;
	WGPT = summary_param.WGPT;
	WWPR = summary_param.WWPR;
	WWPT = summary_param.WWPT;
	WGIR = summary_param.WGIR;
	WGIT = summary_param.WGIT;
	WWIR = summary_param.WWIR;
	WWIT = summary_param.WWIT;

	WBHP = summary_param.WBHP;

	BPR = summary_param.BPR;

	cout << "Summary::InputParam" << endl;
}

void Summary::Setup(const Reservoir& reservoir)
{
	Sumdata.push_back(SumPair("TIME", "  ", "DAY"));
	Sumdata.push_back(SumPair("NRiter", "  ", "  "));
	Sumdata.push_back(SumPair("LSiter", "  ", "  "));
	if (FPR)
		Sumdata.push_back(SumPair("FPR", "  ", "PSIA"));
	if (FOPR)
		Sumdata.push_back(SumPair("FOPR", "  ", "STB/DAY"));
	if (FOPT)
		Sumdata.push_back(SumPair("FOPT", "  ", "STB"));
	if (FGPR)
		Sumdata.push_back(SumPair("FGPR", "  ", "MSCF/DAY"));
	if (FGPt)
		Sumdata.push_back(SumPair("FGPT", "  ", "MSCF"));
	if (FWPR)
		Sumdata.push_back(SumPair("FWPR", "  ", "STB/DAY"));
	if (FWPT)
		Sumdata.push_back(SumPair("FWPT", "  ", "STB"));
	if (FGIR)
		Sumdata.push_back(SumPair("FGIR", "  ", "MSCF/DAY"));
	if (FGIT)
		Sumdata.push_back(SumPair("FGIT", "  ", "MSCF"));
	if (FWIR)
		Sumdata.push_back(SumPair("FWIR", "  ", "STB/DAY"));
	if (FWIT)
		Sumdata.push_back(SumPair("FWIT", "  ", "STB"));

	USI wellnum = reservoir.wellgroup.GetWellNum();

	if (WOPR.activity) {
		if (WOPR.obj[0] == "All") {
			for (USI w = 0; w < wellnum; w++) {
				Sumdata.push_back(SumPair("WOPR", reservoir.wellgroup.GetWellName(w), "STB/DAY"));
				WOPR.index.push_back(w);
			}
		}
		else {
			USI num = WOPR.obj.size();
			for (USI w = 0; w < num; w++) {
				Sumdata.push_back(SumPair("WOPR", WOPR.obj[w], "STB/DAY"));
				WOPR.index.push_back(reservoir.wellgroup.GetIndex(WOPR.obj[w]));
			}
		}
	}
	
	if (WOPT.activity) {
		if (WOPT.obj[0] == "All") {
			for (USI w = 0; w < wellnum; w++) {
				Sumdata.push_back(SumPair("WOPT", reservoir.wellgroup.GetWellName(w), "STB"));
				WOPT.index.push_back(w);
			}
		}
		else {
			USI num = WOPT.obj.size();
			for (USI w = 0; w < num; w++) {
				Sumdata.push_back(SumPair("WOPT", WOPT.obj[w], "STB"));
				WOPT.index.push_back(reservoir.wellgroup.GetIndex(WOPT.obj[w]));
			}
		}
	}

	if (WGPR.activity) {
		if (WGPR.obj[0] == "All") {
			for (USI w = 0; w < wellnum; w++) {
				Sumdata.push_back(SumPair("WGPR", reservoir.wellgroup.GetWellName(w), "MSCF/DAY"));
				WGPR.index.push_back(w);
			}
		}
		else {
			USI num = WGPR.obj.size();
			for (USI w = 0; w < num; w++) {
				Sumdata.push_back(SumPair("WGPR", WGPR.obj[w], "MSCF/DAY"));
				WGPR.index.push_back(reservoir.wellgroup.GetIndex(WGPR.obj[w]));
			}
		}
	}

	if (WGPT.activity) {
		if (WGPT.obj[0] == "All") {
			for (USI w = 0; w < wellnum; w++) {
				Sumdata.push_back(SumPair("WGPT", reservoir.wellgroup.GetWellName(w), "MSCF"));
				WGPT.index.push_back(w);
			}
		}
		else {
			USI num = WGPT.obj.size();
			for (USI w = 0; w < num; w++) {
				Sumdata.push_back(SumPair("WGPT", WGPT.obj[w], "MSCF"));
				WGPT.index.push_back(reservoir.wellgroup.GetIndex(WGPT.obj[w]));
			}
		}
	}

	if (WWPR.activity) {
		if (WWPR.obj[0] == "All") {
			for (USI w = 0; w < wellnum; w++) {
				Sumdata.push_back(SumPair("WWPR", reservoir.wellgroup.GetWellName(w), "STB/DAY"));
				WWPR.index.push_back(w);
			}
		}
		else {
			USI num = WWPR.obj.size();
			for (USI w = 0; w < num; w++) {
				Sumdata.push_back(SumPair("WWPR", WWPR.obj[w], "STB/DAY"));
				WWPR.index.push_back(reservoir.wellgroup.GetIndex(WWPR.obj[w]));
			}
		}
	}

	if (WWPT.activity) {
		if (WWPT.obj[0] == "All") {
			for (USI w = 0; w < wellnum; w++) {
				Sumdata.push_back(SumPair("WWPT", reservoir.wellgroup.GetWellName(w), "STB"));
				WWPT.index.push_back(w);
			}
		}
		else {
			USI num = WWPT.obj.size();
			for (USI w = 0; w < num; w++) {
				Sumdata.push_back(SumPair("WWPT", WWPT.obj[w], "STB"));
				WWPT.index.push_back(reservoir.wellgroup.GetIndex(WWPT.obj[w]));
			}
		}
	}

	if (WGIR.activity) {
		if (WGIR.obj[0] == "All") {
			for (USI w = 0; w < wellnum; w++) {
				Sumdata.push_back(SumPair("WGIR", reservoir.wellgroup.GetWellName(w), "MSCF/DAY"));
				WGIR.index.push_back(w);
			}
		}
		else {
			USI num = WGIR.obj.size();
			for (USI w = 0; w < num; w++) {
				Sumdata.push_back(SumPair("WGIR", WGIR.obj[w], "MSCF/DAY"));
				WGIR.index.push_back(reservoir.wellgroup.GetIndex(WGIR.obj[w]));
			}
		}
	}

	if (WGIT.activity) {
		if (WGIT.obj[0] == "All") {
			for (USI w = 0; w < wellnum; w++) {
				Sumdata.push_back(SumPair("WGIT", reservoir.wellgroup.GetWellName(w), "MSCF"));
				WGIT.index.push_back(w);
			}
		}
		else {
			USI num = WGIT.obj.size();
			for (USI w = 0; w < num; w++) {
				Sumdata.push_back(SumPair("WGIT", WGIT.obj[w], "MSCF"));
				WGIT.index.push_back(reservoir.wellgroup.GetIndex(WGIT.obj[w]));
			}
		}
	}

	if (WWIR.activity) {
		if (WWIR.obj[0] == "All") {
			for (USI w = 0; w < wellnum; w++) {
				Sumdata.push_back(SumPair("WWIR", reservoir.wellgroup.GetWellName(w), "STB/DAY"));
				WWIR.index.push_back(w);
			}
		}
		else {
			USI num = WWIR.obj.size();
			for (USI w = 0; w < num; w++) {
				Sumdata.push_back(SumPair("WWIR", WWIR.obj[w], "STB/DAY"));
				WWIR.index.push_back(reservoir.wellgroup.GetIndex(WWIR.obj[w]));
			}
		}
	}

	if (WWIT.activity) {
		if (WWIT.obj[0] == "All") {
			for (USI w = 0; w < wellnum; w++) {
				Sumdata.push_back(SumPair("WWIT", reservoir.wellgroup.GetWellName(w), "STB"));
				WWIT.index.push_back(w);
			}
		}
		else {
			USI num = WWIT.obj.size();
			for (USI w = 0; w < num; w++) {
				Sumdata.push_back(SumPair("WWIT", WWIT.obj[w], "STB"));
				WWIT.index.push_back(reservoir.wellgroup.GetIndex(WWIT.obj[w]));
			}
		}
	}

	if (WBHP.activity) {
		if (WBHP.obj[0] == "All") {
			for (USI w = 0; w < wellnum; w++) {
				Sumdata.push_back(SumPair("WBHP", reservoir.wellgroup.GetWellName(w), "PSIA"));
				WBHP.index.push_back(w);
			}
		}
		else {
			USI num = WBHP.obj.size();
			for (USI w = 0; w < num; w++) {
				Sumdata.push_back(SumPair("WBHP", WBHP.obj[w], "PSIA"));
				WBHP.index.push_back(reservoir.wellgroup.GetIndex(WBHP.obj[w]));
			}
		}
	}

	if (BPR.activity) {
		USI n = BPR.obj.size();
		for (USI i = 0; i < n; i++) {
			string temp = "( " + to_string(BPR.obj[i].I) + ", "
				+ to_string(BPR.obj[i].J) + ", "
				+ to_string(BPR.obj[i].K) + " )";
			Sumdata.push_back(SumPair("BPR", temp, "PSIA"));
			USI I = BPR.obj[i].I - 1;
			USI J = BPR.obj[i].J - 1;
			USI K = BPR.obj[i].K - 1;
			BPR.index.push_back(reservoir.grid.GetActIndex(I, J, K));
		}
	}

	cout << "Summary::Setup" << endl;

}

void Summary::SetVal(const Reservoir& rs, const OCP_Control& ctrl)
{
	USI n = 0;

	// TIME
	Sumdata[n++].val.push_back(ctrl.GetCurTime());
	// NRiter
	Sumdata[n++].val.push_back(ctrl.GetNRiter());
	// LSiter
	Sumdata[n++].val.push_back(ctrl.GetLSiter());

	// FPR
	if (FPR)
		Sumdata[n++].val.push_back(rs.bulk.CalFPR());
	if (FOPR)
		Sumdata[n++].val.push_back(rs.wellgroup.GetFOPR());
	if (FOPT)
		Sumdata[n++].val.push_back(rs.wellgroup.GetFOPT());
	if (FGPR)
		Sumdata[n++].val.push_back(rs.wellgroup.GetFGPR());
	if (FGPt)
		Sumdata[n++].val.push_back(rs.wellgroup.GetFGPT());
	if (FWPR)
		Sumdata[n++].val.push_back(rs.wellgroup.GetFWPR());
	if (FWPT)
		Sumdata[n++].val.push_back(rs.wellgroup.GetFWPT());
	if (FGIR)
		Sumdata[n++].val.push_back(rs.wellgroup.GetFGIR());
	if (FGIT)
		Sumdata[n++].val.push_back(rs.wellgroup.GetFGIT());
	if (FWIR)
		Sumdata[n++].val.push_back(rs.wellgroup.GetFWIR());
	if (FWIT)
		Sumdata[n++].val.push_back(rs.wellgroup.GetFWIT());

	USI len = 0;
	// WOPR
	len = WOPR.index.size();
	for (USI w = 0; w < len; w++)
		Sumdata[n++].val.push_back(rs.wellgroup.GetWOPR(WOPR.index[w]));
	// WOPT
	len = WOPT.index.size();
	for (USI w = 0; w < len; w++)
		Sumdata[n++].val.push_back(rs.wellgroup.GetWOPT(WOPT.index[w]));
	// WGPR
	len = WGPR.index.size();
	for (USI w = 0; w < len; w++)
		Sumdata[n++].val.push_back(rs.wellgroup.GetWGPR(WGPR.index[w]));
	// WGPT
	len = WGPT.index.size();
	for (USI w = 0; w < len; w++)
		Sumdata[n++].val.push_back(rs.wellgroup.GetWGPT(WGPT.index[w]));
	// WWPR
	len = WWPR.index.size();
	for (USI w = 0; w < len; w++)
		Sumdata[n++].val.push_back(rs.wellgroup.GetWWPR(WWPR.index[w]));
	// WWPT
	len = WWPT.index.size();
	for (USI w = 0; w < len; w++)
		Sumdata[n++].val.push_back(rs.wellgroup.GetWWPT(WWPT.index[w]));
	// WGIR
	len = WGIR.index.size();
	for (USI w = 0; w < len; w++)
		Sumdata[n++].val.push_back(rs.wellgroup.GetWGIR(WGIR.index[w]));
	// WGIT
	len = WGIT.index.size();
	for (USI w = 0; w < len; w++)
		Sumdata[n++].val.push_back(rs.wellgroup.GetWGIT(WGIT.index[w]));
	// WWIR
	len = WWIR.index.size();
	for (USI w = 0; w < len; w++)
		Sumdata[n++].val.push_back(rs.wellgroup.GetWWIR(WWIR.index[w]));
	// WWIT
	len = WWIT.index.size();
	for (USI w = 0; w < len; w++)
		Sumdata[n++].val.push_back(rs.wellgroup.GetWWIT(WWIT.index[w]));
	// WBHP
	len = WBHP.index.size();
	for (USI w = 0; w < len; w++)
		Sumdata[n++].val.push_back(rs.wellgroup.GetWBHP(WBHP.index[w]));

	// BPR
	len = BPR.index.size();
	for (USI i = 0; i < len; i++)
		Sumdata[n++].val.push_back(rs.bulk.GetP(BPR.index[i]));

}

void Summary::PrintInfo(const string& dir)
{
	string FileOut = dir + "SUMMARY.dat";
	ofstream outF(FileOut);
	if (!outF.is_open()) {
		ERRORcheck("Can not open " + FileOut);
		exit(0);
	}
	
	USI ns = 10;
	USI col = 10;
	USI row = 0;
	USI num = Sumdata.size();
	USI len = Sumdata[0].val.size();
	USI id = 0;
	USI ID = 1;
	
	while (id != num) {
		 
		outF << "The " << ++row << "th Row\n";

		// Item
		// Time
		outF << "\t" << setw(10) << Sumdata[0].Item;

		id = ID;
		for (USI i = 1; i < col; i++) {
			outF << "\t" << setw(ns) << Sumdata[id].Item;
			id++;
			if (id == num)
				break;
		}
		outF << "\n";

		// Unit
		// Time
		outF << "\t" << setw(10) << Sumdata[0].Unit;

		id = ID;
		for (USI i = 1; i < col; i++) {
			outF << "\t" << setw(ns) << Sumdata[id].Unit;
			id++;
			if (id == num)
				break;
		}
		outF << "\n";

		// Obj
		// Time
		outF << "\t" << setw(ns) << Sumdata[0].Obj;

		id = ID;
		for (USI i = 1; i < col; i++) {
			outF << "\t" << setw(ns) << Sumdata[id].Obj;
			id++;
			if (id == num)
				break;
		}
		outF << "\n";

		// data
		for (USI l = 0; l < len; l++) {

			// Time
			outF << "\t" << setw(ns) << Sumdata[0].val[l];

			id = ID;
			for (USI i = 1; i < col; i++) {
				outF << "\t" << setw(ns) << Sumdata[id].val[l];
				id++;
				if (id == num)
					break;
			}
			outF << "\n";
		}
		

		ID += (col - 1);
	}

	outF.close();
}

void OCP_Output::InputParam(ParamOutput& param_Output)
{
	summary.InputParam(param_Output.summary);
}

void OCP_Output::Setup(Reservoir& reservoir, string& dir)
{
	Dir = dir;
	summary.Setup(reservoir);
}

void OCP_Output::SetVal(const Reservoir& reservoir, const OCP_Control& ctrl)
{
	summary.SetVal(reservoir, ctrl);
}

void OCP_Output::PrintInfo()
{
	summary.PrintInfo(Dir);
}

/*----------------------------------------------------------------------------*/
/*  Brief Change History of This File                                         */
/*----------------------------------------------------------------------------*/
/*  Author              Date             Actions                              */
/*----------------------------------------------------------------------------*/
/*  Shizhe Li           Oct/01/2021      Create file                          */
/*----------------------------------------------------------------------------*/
