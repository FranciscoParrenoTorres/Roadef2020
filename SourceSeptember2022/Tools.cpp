
#include "Maintenance.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

using json = nlohmann::json;


using namespace std;


class sax_event_consumer : public json::json_sax_t
{
  public:
    std::vector<std::string> events;

    bool null() override
    {
        events.push_back("value: null");
        return true;
    }

    bool boolean(bool val) override
    {
        events.push_back("value: " + std::string(val ? "true" : "false"));
        return true;
    }

    bool number_integer(number_integer_t val) override
    {
        events.push_back("value: " + std::to_string(val));
        return true;
    }

    bool number_unsigned(number_unsigned_t val) override
    {
        events.push_back("value: " + std::to_string(val));
        return true;
    }

    bool number_float(number_float_t val, const string_t& s) override
    {
        events.push_back("value: " + s);
        return true;
    }

    bool string(string_t& val) override
    {
        events.push_back("value: " + val);
        return true;
    }

    bool start_object(std::size_t elements) override
    {
        events.push_back("start: object");
        return true;
    }

    bool end_object() override
    {
        events.push_back("end: object");
        return true;
    }

    bool start_array(std::size_t elements) override
    {
        events.push_back("start: array");
        return true;
    }

    bool end_array() override
    {
        events.push_back("end: array");
        return true;
    }

    bool key(string_t& val) override
    {
        events.push_back("key: " + val);
        return true;
    }

    bool binary(json::binary_t& val) override
    {
        events.push_back("binary");
        return true;
    }

    bool parse_error(std::size_t position, const std::string& last_token, const json::exception& ex) override
    {
        events.push_back("error: " + std::string(ex.what()));
        return false;
    }
};

bool comp_descending(double a, double b)
{
	return (a > b);
}

/*
//vectorized counting selection, vectorization along both directions
double  get_kth_count_v16both(const double* arr, int n, int idx) {
	__m128i idxV = _mm_set1_epi16(idx);
	for (size_t i = 0; i < n; i += 8) {
		auto xx = _mm_loadu_si128((__m128i*) & arr[i]);

		__m128i cntLess = _mm_setzero_si128();
		__m128i cntEq = _mm_setzero_si128();
		for (size_t j = 0; j < n; j += 8) {
			__m128i vj = _mm_loadu_si128((__m128i*) & arr[j]);

			__m128i a0 = _mm_unpacklo_epi16(vj, vj);
			__m128i a1 = _mm_unpackhi_epi16(vj, vj);
			__m128i b0 = _mm_unpacklo_epi32(a0, a0);
			__m128i b1 = _mm_unpackhi_epi32(a0, a0);
			__m128i b2 = _mm_unpacklo_epi32(a1, a1);
			__m128i b3 = _mm_unpackhi_epi32(a1, a1);
			__m128i vAll0 = _mm_unpacklo_epi32(b0, b0);
			__m128i vAll1 = _mm_unpackhi_epi32(b0, b0);
			__m128i vAll2 = _mm_unpacklo_epi32(b1, b1);
			__m128i vAll3 = _mm_unpackhi_epi32(b1, b1);
			__m128i vAll4 = _mm_unpacklo_epi32(b2, b2);
			__m128i vAll5 = _mm_unpackhi_epi32(b2, b2);
			__m128i vAll6 = _mm_unpacklo_epi32(b3, b3);
			__m128i vAll7 = _mm_unpackhi_epi32(b3, b3);

#define DOIT(k) \
                cntLess = _mm_sub_epi16(cntLess, _mm_cmplt_epi16(vAll##k, xx)); \
                cntEq = _mm_sub_epi16(cntEq, _mm_cmpeq_epi16(vAll##k, xx));
			DOIT(0);
			DOIT(1);
			DOIT(2);
			DOIT(3);
			DOIT(4);
			DOIT(5);
			DOIT(6);
			DOIT(7);
#undef DOIT
		}

		__m128i mask = _mm_andnot_si128(_mm_cmplt_epi16(idxV, cntLess), _mm_cmplt_epi16(idxV, _mm_add_epi16(cntLess, cntEq)));
		if (int bm = _mm_movemask_epi8(mask)) {
			for (int t = 0; t < 8; t++)
				if (bm & (1 << (2 * t)))
					return arr[i + t];
		}
	}
	assert(0);
	return -1;
}*/




void Maintenance::leer_problem(string& name) {
	// open the json file - here replaced with a istringstream containing the json data




	string name2 = name + ".json";
	ifstream ifs(name2);
	if (!ifs.is_open()) {

		cout << "El fichero " << name << "está abierto" << endl;
		return ;
	}
	    sax_event_consumer sec;


	json j = json::parse(ifs);
//	bool read= json::sax_parse(ifs,sec);
	//T
	if (j.find("T") != j.end()) {
		json info = j["T"];
		M_T = info;

	}
	//Quantile
	if (j.find("Quantile") != j.end()) {
		json info = j["Quantile"];
		M_Quantile = info;
	}
	//Alpha
	if (j.find("Alpha") != j.end()) {
		json info = j["Alpha"];
		M_Alpha = info;
	}
	cout << M_T << M_Quantile << M_Alpha;
	M_T_Season.clear();
	for (int i = 0; i < M_T; i++)
		M_T_Season.push_back(-1);
	// Seasons

	if (j.find("Scenarios_number") != j.end()) {
		M_Total_Scenarios = 0;
		M_Scenarios_number = j["Scenarios_number"].get<vector<int>>();
		for (size_t i = 0; i < M_Scenarios_number.size(); i++) {
			if (M_Scenarios_number[i] > 1)
				M_Pool = true;
			M_Total_Scenarios += M_Scenarios_number[i];
		}
	}

	if (j.find("Seasons") != j.end())
	{
		json info = j["Seasons"];
		size_t i=0;
		for (json::iterator it = info.begin(); it != info.end(); ++it)
		{
			M_S_Id.insert(pair<string, int>(it.key(), i));
			M_S_Name.push_back(it.key());
			Season S(i);

			S.Set_Name(it.key());

			if ((*it).is_number())
			{
				vector<int> intNumbers = ((*it).get<vector<int>>());
				S.Set_Numbers(intNumbers);
			}
			else
			{
				vector <string> kk = (*it);
				vector < int> intNumbers;
				for (int ik = 0; ik < kk.size(); ik++)
				{
					int num = atoi(kk.at(ik).c_str());
					intNumbers.push_back(num);
				}
				S.Set_Numbers(intNumbers);
			}
			for (vector<int>::iterator it2 = S.Get_Numbers().begin(); it2 != S.Get_Numbers().end(); it2++)
			{
				M_T_Season[(*it2)-1] = i;
			}
			M_Seasons.push_back(S);
			i++;
		}
	}
	// Resources


	if (j.find("Resources") != j.end())
	{
		json info = j["Resources"];

		// verify that "name" attribute exists in info
		size_t i=0;
		for (json::iterator it = info.begin(); it != info.end(); ++it)
		{
			//           string kk = it.key().substr(11, it.key().size() - 11);
					   //            exclusion_int.push_back(atoi(kk.c_str()));
			Resource R(i);
			R.Set_Name(it.key());
			vector<double> vec(M_T);
			vector<double> vec2(M_T);
			R.Set_Temp(vec);
			R.Set_Copy(vec2);
			M_R_Name.push_back(it.key());
			bool abajo = false;
			M_R_Id.insert(pair<string, int>(it.key(), i));

			if ((*it).find("max") != (*it).end())
			{
				vector<double> max = (*it)["max"].get<vector<double>>();
				R.Set_Upper(max);

				//              cout << (*it)["max"]; // upper

			}
			if ((*it).find("min") != (*it).end())
			{
				vector<double> min = (*it)["min"].get<vector<double>>();
				R.Set_Lower(min);
				//                cout << (*it)["min"]; // upper

			}
			for (vector<double> ::iterator it= R.Get_Lower().begin(); it != R.Get_Lower().end(); it++)
			{
				bool abajo = false;
				if ((*it) > __FLT_EPSILON__)
					abajo = true;
				M_Lower_ResourcesV.push_back(abajo);
			}
			M_Resources.push_back(R);
			M_N_Resources++;
			i++;



		}
		info.clear();
	}

	// Interventions

	if (j.find("Interventions") != j.end())
	{
		json info = j["Interventions"];
		int sizei = info.size();
		// cout << "número de interventions" << sizei << endl;
		// verify that "name" attribute exists in info
		size_t i=0;
		int cont = 0;
		for (json::iterator it = info.begin(); it != info.end(); ++it)
		{
			Intervention I(i);
			I.Set_Name(it.key());
			M_I_Id.insert(pair<string, int>(it.key(), i));
			M_I_Name.push_back(it.key());
			cont++;
			//           string kk = it.key().substr(13, it.key().size() - 13);
		   //            exclusion_int.push_back(atoi(kk.c_str()));


			if ((*it).find("tmax") != (*it).end())
			{
				if ((*it)["tmax"].is_number())
				{
					I.Set_Tmax((*it)["tmax"]);
				}
				else
				{
					string  kk = (*it)["tmax"];
					//Change 11/04/2021
//					I.Set_Tmax(atoi(kk.c_str())-1);
					I.Set_Tmax(atoi(kk.c_str()));
				}

				//               cout << I.t_max; // upper


			}
			if ((*it).find("Delta") != (*it).end())
			{
				vector<int> deltas = (*it)["Delta"].get<vector<int>>();
				I.Set_Delta(deltas);
				//              cout << (*it)["Delta"]; // upper
				I.Set_Min_Delta(*min_element(I.Get_Delta().begin(), I.Get_Delta().end()));

			}


			if ((*it).find("workload") != (*it).end())
			{
				vector<int> workload_res;
				map<int, map<int, map<int, double>>> workload;

				//           vector<int> workload_r;
				json info3 = (*it)["workload"];

				for (json::iterator it2 = info3.begin(); it2 != info3.end(); ++it2)
				{
					//                 workload_r.push_back(M_R_Id[it2.key()]);
					
					map<int, map<int, double >> work_res;
					//Resource
					json info4 = info3[it2.key()];
					string resource =it2.key();
					for (json::iterator it3 = info4.begin(); it3 != info4.end(); ++it3)
					{
						//Time  current  
						map<int, double >  work_sea;
						json info5 = info4[it3.key()];
						for (json::iterator it4 = info5.begin(); it4 != info5.end(); ++it4)
						{
							//Time ini
							json info6 = info5[it4.key()];
							work_sea.insert(pair<int, double >(atoi(it4.key().c_str()) - 1, info6));



						}
						work_res.insert(pair<int, map<int, double >>(atoi(it3.key().c_str()) - 1, work_sea));

					}
					workload.insert(pair<int, map<int, map<int, double >>>(M_R_Id[it2.key()], work_res));
					workload_res.push_back(M_R_Id[resource]);
//					I.Set_Workload(workload);
					//                   I.Set_WorkloadR(workload_r);


									//   I.Set_WorkloadT(workload_r);

				}
				vector<vector<vector<double>>> workload_N(M_N_Resources);

				//Poner en vector
				for (size_t i = 0; i < M_N_Resources; i++)
				{
					vector<vector<double>> workload_N2;
					if (find(workload_res.begin(), workload_res.end(), i) !=
					    workload_res.end())
					{
						//							vector<vector<double> > workload_N2(I.Get_Tmax());
						for (int t = 0; t < I.Get_Tmax(); t++)
						{
							vector<double> workload_N3(I.Get_Delta(t));
							int cont = 0;
							for (int t2 = t; t2 < t + I.Get_Delta(t);
							     t2++)
							{

								if ((workload.find(i) !=
								     workload.end()) &&
								    (workload[i].find(t2) !=
								     workload[i].end()) &&
								    (workload[i][t2].find(t) !=
								     workload[i][t2].end()))

									//			workload_N3.push_back(workload[i][t2][t]);
									//		else
									//			workload_N3.push_back(0);
									workload_N3[cont] =
									    (workload[i][t2][t]);
								else
									workload_N3[cont] = 0;
								cont++;
							}
							//		workload_N2.push_back(workload_N3);
							workload_N2.push_back(workload_N3);
						}
					}
					//		workload_N.push_back(workload_N2);
					workload_N[i] = (workload_N2);
				}
				I.Set_Workload_Res(workload_res);
				//					I.Set_Workload(workload);
				I.Set_WorkloadN(workload_N);

				//Accumulative workload
				vector<double> vec_workloadC(M_T);

				for (map<int, map<int, map<int, double>>>::iterator itw =
				         workload.begin();
				     itw != workload.end(); itw++)
				{

					for (map<int, map<int, double>>::iterator itw2 =
					         itw->second.begin();
					     itw2 != itw->second.end(); itw2++)
					{

						for (map<int, double>::iterator itw3 =
						         itw2->second.begin();
						     itw3 != itw2->second.end(); itw3++)
						{

							vec_workloadC[itw3->first] +=
							    ((itw3->second) /
							     (M_Resources[(*itw).first]
							          .Get_Upper()[(*itw2).first]));
						}
					}
				}
				//					for (int ik = 0; ik < M_T; ik++)
				vector<double> tempWxt(vec_workloadC.size());
				for (size_t i = 0; i < vec_workloadC.size(); i++)
				{
					tempWxt[i] = vec_workloadC[i] / I.Get_Delta()[i];
				}
				I.Set_WorkloadCxT(tempWxt);
				I.Set_WorkloadC(vec_workloadC);
				I.Set_Min_WorkloadxT(tempWxt[Min(tempWxt)]);
				I.Set_Max_WorkloadxT(tempWxt[Max(tempWxt)]);
				I.Set_Min_Workload(vec_workloadC[Min(vec_workloadC)]);
				I.Set_Max_Workload(vec_workloadC[Max(vec_workloadC)]);
			}
			if ((*it).find("risk") != (*it).end())
			{
				map<int, map<int, vector<double>>> risk;
				json info3 = (*it)["risk"];
				for (json::iterator it2 = info3.begin(); it2 != info3.end(); ++it2)
				{
					//SEason
					map<int, vector<double>> risk_sea;
					json info4 = info3[it2.key()];
					for (json::iterator it3 = info4.begin(); it3 != info4.end();
					     ++it3)
					{

						//Time
						json info5 = info4[it3.key()];
						//Scenarios
						vector<double> temp = info5.get<vector<double>>();

						temp.push_back(
						    accumulate(temp.begin(), temp.end(), 0.0));

						risk_sea.insert(pair<int, vector<double>>(
						    atoi(it3.key().c_str()) - 1, temp));
					}
					risk.insert(pair<int, map<int, vector<double>>>(
					    atoi(it2.key().c_str()) - 1, risk_sea));
				}
				vector<vector<vector<double>>> riskN;
				//					vector<vector<double>> risk_actualN;
				//					vector<double> risk_scenarios;

				//Riesgo acumulado por día de inicio
				vector<double> vec_riskC(M_T);
				vector<double> vec_riskCxT(M_T);
				vector<double> vec_riskQuantile(M_T);
				vector<double> vec_riskExcess(M_T);
				vector<vector<double>> mat_riskexcess;
				//			vector<double> vec_riskMC;

				//Poner en vector
				for (int ta = 0; ta < I.Get_Tmax(); ta++)
				{
					vector<vector<double>> risk_actualN;
					vector<double> vec_riskexcess(I.Get_Delta(ta));
					for (int ta2 = ta; ta2 < ta + I.Get_Delta(ta); ta2++)
					{

						vector<double> risk_scenarios(
						    M_Scenarios_number[ta2]);
						double acumulado = 0;
						vector<double> temp(M_Scenarios_number[ta2]);
						for (int t2 = 0; t2 < M_Scenarios_number[ta2]; t2++)
						{
							if ((risk.find(ta2) != risk.end()) &&
							    (risk[ta2].find(ta) != risk[ta2].end()))
								risk_scenarios[t2] =
								    (risk[ta2][ta]
								         [t2]); // [ta2] [ta] [t2] );
							else
								risk_scenarios[t2] = (0);

							//									risk_scenarios.push_back(risk[ta2][ta][t2]);// [ta2] [ta] [t2] );
							//								else
							//									risk_scenarios.push_back(0);
							acumulado += risk_scenarios[t2];
							//
							//temp.push_back(risk_scenarios[t2]);
							temp[t2] = risk_scenarios[t2];
						}
						double media = acumulado / M_Scenarios_number[ta2];
						vec_riskC[ta] +=
						    acumulado / M_Scenarios_number[ta2];
						if (M_Scenarios_number[ta2] > 1)
						{
							int pos = ceil(M_Scenarios_number[ta2] *
							               M_Quantile) -
							          1;

							nth_element(temp.begin(),
							            temp.begin() + pos, temp.end());
							//		double cuantil1= M_T_Temp_Risk_S[i][pos];
							//		 sort(M_T_Temp_Risk_S[i].begin(), M_T_Temp_Risk_S[i].end());
							vec_riskQuantile[ta] += temp[pos];
							vec_riskExcess[ta] +=
							    max((double)0, (temp[pos] - media));
							vec_riskexcess[ta2 - ta] =
							    max((double)0, (temp[pos] - media));
							if (vec_riskexcess[ta2 - ta] <
							    -1 * __FLT_EPSILON__)
								Problema(111);
						}
						else
						{
							vec_riskQuantile[ta] = vec_riskC[ta];
							vec_riskExcess[ta] = 0;
							vec_riskexcess[ta2 - ta] = 0;
						}
						risk_actualN.push_back(risk_scenarios);
					}
					mat_riskexcess.push_back(vec_riskexcess);
					riskN.push_back(risk_actualN);
				}
				//					I.Set_Workload(workload);
				I.Set_RiskExcessXt(mat_riskexcess);
				//Quito para ahorrar memoria
				//					I.Set_Risk(risk);
				I.Set_RiskN(riskN);

				for (size_t i = 0; i < vec_riskCxT.size(); i++)
				{
					vec_riskCxT[i] = vec_riskC[i] / I.Get_Delta()[i];
				}
				I.Set_RiskExcess(vec_riskExcess);
				I.Set_RiskC(vec_riskC);
				I.Set_RiskCxT(vec_riskCxT);
				I.Set_RiskCQuantile(vec_riskQuantile);
				I.Set_Min_RiskxT(vec_riskCxT[Min(vec_riskCxT)]);
				I.Set_Max_RiskxT(vec_riskCxT[Max(vec_riskCxT)]);

				I.Set_Max_Risk(vec_riskC[Max(vec_riskC)]);
				I.Set_Max_RiskQ(vec_riskQuantile[Max(vec_riskQuantile)]);
				vector<int> T_ordenados_Risk;
				vector<int> T_ordenados_RiskQ;
				//En T ordenados deben estar todos los que entran
				//hay algunos que entran solos
				for (int i1 = 0; i1 < (I.Get_Tmax()); i1++)
				{
					if (PuedoPonerSola(I, i1) == false)
						continue;
					bool colocado1 = false;
					for (int i2 = 0;
					     i2 < T_ordenados_Risk.size() && colocado1 == false;
					     i2++)
					{
						if (vec_riskC[i1] <
						    (vec_riskC[T_ordenados_Risk[i2]] -
						     __FLT_EPSILON__))
						{
							T_ordenados_Risk.insert(
							    T_ordenados_Risk.begin() + i2, i1);
							colocado1 = true;
						}
					}

					if (colocado1 == false)
						T_ordenados_Risk.push_back(i1);
					bool colocado2 = false;
					for (int i2 = 0;
					     i2 < T_ordenados_RiskQ.size() && colocado2 == false;
					     i2++)
					{
						if (vec_riskQuantile[i1] <
						    (vec_riskQuantile[T_ordenados_RiskQ[i2]] -
						     __FLT_EPSILON__))
						{
							T_ordenados_RiskQ.insert(
							    T_ordenados_RiskQ.begin() + i2, i1);
							colocado2 = true;
						}
					}

					if (colocado2 == false)
						T_ordenados_RiskQ.push_back(i1);
				}
				I.Set_Min_Risk(vec_riskC[T_ordenados_Risk[0]]);
				I.Set_Min_RiskQ(vec_riskQuantile[T_ordenados_RiskQ[0]]);
				if (T_ordenados_Risk.size() > 1)
					I.Set_Regret(vec_riskC[T_ordenados_Risk[1]] -
					             vec_riskC[T_ordenados_Risk[0]]);
				else
					I.Set_Regret(0);
				if (T_ordenados_RiskQ.size() > 1)
					I.Set_RegretQ(vec_riskQuantile[T_ordenados_RiskQ[1]] -
					              vec_riskQuantile[T_ordenados_RiskQ[0]]);
				else
					I.Set_RegretQ(0);
				//					I.Set_Max_Risk(vec_riskC[T_ordenados_Risk[T_ordenados_Risk.size()-1]]);
				double val1 = vec_riskC[T_ordenados_Risk[0]];
				list<int> T_ordenados_Risk_I;
				for (int i = 1; i < T_ordenados_Risk.size(); i++)
				{
					if (vec_riskC[T_ordenados_Risk[i]] >
					    (val1 + __FLT_EPSILON__))
					{
						val1 = vec_riskC[T_ordenados_Risk[i]];
						T_ordenados_Risk_I.push_back(i);
					}
				}
				T_ordenados_Risk_I.push_back(T_ordenados_Risk.size());
				val1 = vec_riskQuantile[T_ordenados_RiskQ[0]];
				list<int> T_ordenados_Risk_IQ;
				for (int i = 1; i < T_ordenados_RiskQ.size(); i++)
				{
					if (vec_riskQuantile[T_ordenados_RiskQ[i]] >
					    (val1 + __FLT_EPSILON__))
					{
						val1 = vec_riskQuantile[T_ordenados_RiskQ[i]];
						T_ordenados_Risk_IQ.push_back(i);
					}
				}
				T_ordenados_Risk_IQ.push_back(T_ordenados_RiskQ.size());
				I.Set_T_ordenados_Risk_IQ(T_ordenados_Risk_IQ);
				I.Set_T_ordenados_RiskQ(T_ordenados_RiskQ);
				I.Set_T_ordenados_Risk_I(T_ordenados_Risk_I);
				I.Set_T_ordenados_Risk(T_ordenados_Risk);
			}
			M_N_Interventions = i+1;
			M_Interventions.push_back(I);
			i++;
		}
		info.clear();
	}




    //Exclusion
    if (j.find("Exclusions") != j.end())
    {
        json info = j["Exclusions"];
        int sizei = info.size();
  //      cout << sizei << endl;
        for (json::iterator it = info.begin(); it != info.end(); ++it)
        {
            vector <int> exclusion_int;
            vector <string> exclusion = info[it.key()];
            
            M_Exclusions_Originals.push_back(exclusion);
            exclusion_int.push_back(M_I_Id[exclusion[0]]);
			exclusion_int.push_back(M_I_Id[exclusion[1]]);
			exclusion_int.push_back(M_S_Id[exclusion[2]]);
			M_Interventions[M_I_Id[exclusion[0]]].Add_Exclusion(M_I_Id[exclusion[1]], M_S_Id[exclusion[2]]);
			M_Interventions[M_I_Id[exclusion[1]]].Add_Exclusion(M_I_Id[exclusion[0]], M_S_Id[exclusion[2]]);
			//Convert string to int
            M_Exclusions.push_back(exclusion_int);
            M_N_Exclusions++;
        }

    }
    ifs.close();
	return;
}
void Maintenance::leer_problem_wojson(string& name_file) 
{
	// open the json file - here replaced with a istringstream containing the json data


	struct timeb t1;
	 M_Name_file = name_file;
	string name_file2 = name_file + ".json";
	ifstream ifs(name_file2);
	if (!ifs.is_open()) {

		cout << "El fichero " << name_file2 << "está abierto" << endl;
		return ;
	}
	ftime(&t1);
	M_Time_Initial = t1;




//	std::ifstream fin("input.dat");
	bool T = false;
	bool A = false;
	bool C = false;
	bool S = false;
	bool Q = false;
	while (ifs)
	{
		size_t buffer_size = 1 << 20;
		buffer_size = buffer_size * 400;
		char* buffer = new char[buffer_size];
		// Try to read next chunk of data
		ifs.read(buffer, buffer_size);
		// Get the number of bytes actually read
		size_t count = ifs.gcount();
		// If nothing has been read, break
		if (!count)
			break;
		// Do whatever you need with first count bytes in the buffer
		// ...

		string str = buffer;
		delete[] buffer;
		size_t index = 0;
		size_t posT = str.find("\"T\"", index);
		if (posT != str.npos)
		{
			size_t kk = str.find(":", posT);
			size_t pos = str.find(",", posT);
			string kk3 = str.substr(kk + 1, pos - kk - 1);
			M_T = stoi(kk3);
			T = true;

		}
		size_t  posQ = str.find("Quantile", index);
		if (posQ != str.npos)
		{
			size_t kk = str.find(":", posQ);
			size_t pos = str.find(",", posQ);
			string kk3 = str.substr(kk + 1, pos - kk - 1);
			M_Quantile = stod(kk3);
			Q = true;
		}

		size_t  posA = str.find("Alpha", index);
		if (posA != str.npos)
		{
			size_t kk = str.find(":", posA);
			size_t pos = str.find(",", posA);
			string kk3 = str.substr(kk + 1, pos - kk - 1);
			M_Alpha = stod(kk3);
			A = true;
		}
		size_t  posC = str.find("ComputationTime", index);
		if (posC != str.npos)
		{
			size_t kk = str.find(":", posC);
			size_t pos = str.find(",", posC);
			string kk3 = str.substr(kk + 1, pos - kk - 1);
			M_ComputionTime = stoi(kk3);
			C = true;
		}
		size_t  posS = str.find("Scenarios_number", index);
		if (posS != str.npos)
		{
			int temp;
			M_Total_Scenarios = 0;
			size_t pos = str.find(",", posS);
			size_t pos_b = pos - 5;
			for (int i = 0; i < M_T; i++)
			{


				temp = stoi(str.substr(pos_b, pos - 1));
				pos_b = pos;
				if (temp > 1)
					M_Pool = true;
				M_Total_Scenarios += temp;
				M_Scenarios_number.push_back(temp);
				if (i < (M_T - 2))
				{
					pos = str.find(",", pos + 1);
					pos_b = pos - 5;
				}
				else
				{
					pos_b = pos + 1;
					pos = str.find("]", pos + 1);
				}

			}
			S = true;


		}
		
	}

	string str;
	if (!Q || !S || !T || !A || !C)
	{
		//Primero leemos valores
		ifs.clear();
		ifs.seekg(0, ifs.beg);
		
		while (getline(ifs, str))
		{
			size_t index = 0;
			if (str.find("\"T\"", index) != str.npos)
			{
				size_t kk = str.find(":");
				size_t pos = str.find(",");
				string kk3 = str.substr(kk + 1, pos - kk - 1);
				M_T = stoi(kk3);

			}
			if (str.find("Quantile", index) != str.npos)
			{
				size_t kk = str.find(":");
				size_t pos = str.find(",");
				string kk3 = str.substr(kk + 1, pos - kk - 1);
				M_Quantile = stod(kk3);
			}


			if (str.find("Alpha", index) != str.npos)
			{
				size_t kk = str.find(":");
				size_t pos = str.find(",");
				string kk3 = str.substr(kk + 1, pos - kk - 1);
				M_Alpha = stod(kk3);
			}
			if (str.find("ComputationTime", index) != str.npos)
			{
				size_t kk = str.find(":");
				size_t pos = str.find(",");
				string kk3 = str.substr(kk + 1, pos - kk - 1);
				M_ComputionTime = stoi(kk3);
			}
			if (str.find("Scenarios_number", index) != str.npos)
			{
				int temp;
				M_Total_Scenarios = 0;
				for (int i = 0; i < M_T; i++)
				{
					getline(ifs, str);
					size_t pos = str.find(",");
					temp = stoi(str.substr(1, pos - 1));
					if (temp > 1)
						M_Pool = true;
					M_Total_Scenarios += temp;
					M_Scenarios_number.push_back(temp);

				}


			}
		}
	}
	for (int i = 0; i < M_T; i++)
	{
		M_T_Season.push_back(-1);
	}
	//	settingsFile.seekg(0, settingsFile.beg);
	ifs.clear();
	ifs.seekg(0, ifs.beg);
	string name;
	//Seasons

	bool bSeason = false;
	//Resources
	bool bResources = false;
	bool bexclusions = false;
	while (getline(ifs, str) && (bSeason==false || bResources==false))
	{
		size_t index = 0;

		if (str.find("Seasons\":{", index) != str.npos || str.find("Seasons\":", index) != str.npos)
		{
			bSeason = true;
			int cont = 0;
			do
			{
				getline(ifs, str);
				//Empieza una season
				if (str.find(":[", 0) != str.npos || str.find(": [", 0) != str.npos)
				{
					if (str.find(":[", 0) != str.npos)
					name = str.substr(str.find("\"", 0) + 1, str.find(":[", 0) - str.find("\"", 0) - 2);
					else
						name = str.substr(str.find("\"", 0) + 1, str.find(": [", 0) - str.find("\"", 0) - 2);
					M_S_Id.insert(pair<string, int>(name, cont));
					M_S_Name.push_back(name);
					Season S(cont);

					S.Set_Name(name);
					vector<int> numbers;
					do
					{
						
						getline(ifs, str);
						if (str.size() != 0)
						{

							if (str.find("],") == str.npos && (str.find("]")==str.npos))
							{
								name = str.substr(str.find("\"", 0) + 1, str.find(",", 0) - str.find("\"", 0) - 2);
								numbers.push_back(stoi(name));
							}
						}

					} while (str.find("],") == str.npos && (str.find("]") == str.npos));
					S.Set_Numbers(numbers);

					for (vector<int>::iterator it2 = S.Get_Numbers().begin(); it2 != S.Get_Numbers().end(); it2++)
					{
						M_T_Season[(*it2) - 1] = cont;	
//						M_T_Season[(*it2) ] = cont;

					}
					M_Seasons.push_back(S);
					cont++;

				}
			} while (str.find("],") != str.npos && (str.find("]") != str.npos));
		}
		index = 0;

		if (str.find("Resources", index) != str.npos)
		{
			bResources = true;
			int cont = 0;
			do
			{
				getline(ifs, str);
				//Empieza una season
				if (str.find(":{", 0) != str.npos || str.find(": {", 0) != str.npos)
				{
					if (str.find(":{", 0) != str.npos)
						name = str.substr(str.find("\"", 0) + 1, str.find(":{", 0) - str.find("\"", 0) - 2);
					else
						name = str.substr(str.find("\"", 0) + 1, str.find(": {", 0) - str.find("\"", 0) - 2);



					Resource R(cont);
					R.Set_Name(name);
					vector<double> vec(M_T);
					R.Set_Temp(vec);
					R.Set_Copy(vec);
					M_R_Name.push_back(name);
					M_R_Id.insert(pair<string, int>(name, cont));

					vector<double> min;
					vector<double> max;
					getline(ifs, str); //lee min
					double temp;
					bool abajo = false;
					for (int i = 0; i < M_T; i++)
					{
						getline(ifs, str);
						size_t pos = str.find(",");
						temp = stod(str.substr(1, pos - 1));
						if (temp > __FLT_EPSILON__)
						{
							M_Lower_Resources = true;
							abajo = true;
						}
						min.push_back(temp);

					}
					getline(ifs, str); //lee el fin de la anterior
					getline(ifs, str); //lee max
					for (int i = 0; i < M_T; i++)
					{
						getline(ifs, str);
						size_t pos = str.find(",");
						temp = stod(str.substr(1, pos - 1));
						max.push_back(temp);

					}
					R.Set_Lower(min);
					R.Set_Upper(max);

					M_Lower_ResourcesV.push_back(abajo);
					M_Resources.push_back(R);
					M_N_Resources++;
					cont++;



				}
				getline(ifs, str); //lee el ]
				getline(ifs, str);
			} while (str.find("},") != str.npos);
		}

			

 	}
	if (bResources == false)
	{
		ifs.clear();
		ifs.seekg(0, ifs.beg);
		while (getline(ifs, str) && ( bResources == false))
		{

			size_t index = 0;
			if (str.find("Resources", index) != str.npos)
			{
				bResources = true;
				int cont = 0;
				do
				{
					getline(ifs, str);
					//Empieza una season
					if (str.find(":{", 0) != str.npos || str.find(": {", 0) != str.npos)
					{
						if (str.find(":{", 0) != str.npos)
							name = str.substr(str.find("\"", 0) + 1, str.find(":{", 0) - str.find("\"", 0) - 2);
						else
							name = str.substr(str.find("\"", 0) + 1, str.find(": {", 0) - str.find("\"", 0) - 2);



						Resource R(cont);
						R.Set_Name(name);
						vector<double> vec(M_T);
						R.Set_Temp(vec);
						R.Set_Copy(vec);
						M_R_Name.push_back(name);
						M_R_Id.insert(pair<string, int>(name, cont));

						vector<double> min;
						vector<double> max;
						getline(ifs, str); //lee min
						double temp;
						bool abajo = false;
						for (int i = 0; i < M_T; i++)
						{
							getline(ifs, str);
							size_t pos = str.find(",");
							temp = stod(str.substr(1, pos - 1));
							if (temp > __FLT_EPSILON__)
							{
								M_Lower_Resources = true;
								abajo = true;
							}
							min.push_back(temp);

						}
						getline(ifs, str); //lee el fin de la anterior
						getline(ifs, str); //lee max
						for (int i = 0; i < M_T; i++)
						{
							getline(ifs, str);
							size_t pos = str.find(",");
							temp = stod(str.substr(1, pos - 1));
							max.push_back(temp);

						}
						R.Set_Lower(min);
						R.Set_Upper(max);

						M_Lower_ResourcesV.push_back(abajo);
						M_Resources.push_back(R);
						M_N_Resources++;
						cont++;



					}
					getline(ifs, str); //lee el ]
					getline(ifs, str);
				} while (str.find("},") != str.npos);
			}
		}

	}
	if (bSeason == false)
	{
		ifs.clear();
		ifs.seekg(0, ifs.beg);
		while (getline(ifs, str) && (bSeason == false ))
		{
			size_t index = 0;

			if (str.find("Seasons\":{", index) != str.npos || str.find("Seasons\":", index) != str.npos)
			{
				bSeason = true;
				int cont = 0;
				do
				{
					getline(ifs, str);
					//Empieza una season
					if (str.find(":[", 0) != str.npos || str.find(": [", 0) != str.npos)
					{
						if (str.find(":[", 0) != str.npos)
							name = str.substr(str.find("\"", 0) + 1, str.find(":[", 0) - str.find("\"", 0) - 2);
						else
							name = str.substr(str.find("\"", 0) + 1, str.find(": [", 0) - str.find("\"", 0) - 2);
						M_S_Id.insert(pair<string, int>(name, cont));
						M_S_Name.push_back(name);
						Season S(cont);

						S.Set_Name(name);
						vector<int> numbers;
						do
						{

							getline(ifs, str);
							if (str.size() != 0)
							{

								if (str.find("],") == str.npos && (str.find("]") == str.npos))
								{
									name = str.substr(str.find("\"", 0) + 1, str.find(",", 0) - str.find("\"", 0) - 2);
									numbers.push_back(stoi(name));
								}
							}

						} while (str.find("],") == str.npos && (str.find("]") == str.npos));
						S.Set_Numbers(numbers);

						for (vector<int>::iterator it2 = S.Get_Numbers().begin(); it2 != S.Get_Numbers().end(); it2++)
						{
							M_T_Season[(*it2) - 1] = cont;
							//						M_T_Season[(*it2) ] = cont;

						}
						M_Seasons.push_back(S);
						cont++;

					}
				} while (str.find("],") != str.npos && (str.find("]") != str.npos));
			}
			index = 0;

		}

	}


	//leidos resources
//	ifs.clear();
//	ifs.seekg(0, ifs.beg);
	bool bInterventions = false;
	while (getline(ifs, str) && (bInterventions==false))
	{
		size_t index = 0;
		
		if (str.find("Interventions", index) != str.npos)
		{
			bInterventions = true;
			int cont = 0;
			do
			{
				getline(ifs, str);
				//Empieza una intervention
				if (str.find(":{", 0) != str.npos || str.find(": {", 0) != str.npos)
				{
					if (str.find(":{", 0) != str.npos)
						name = str.substr(str.find("\"", 0) + 1, str.find(":{", 0) - str.find("\"", 0) - 2);
					else
						name = str.substr(str.find("\"", 0) + 1, str.find(": {", 0) - str.find("\"", 0) - 2);
					Intervention I(cont);
					I.Set_Name(name);
					M_I_Id.insert(pair<string, int>(name, cont));
					M_I_Name.push_back(name);
					cont++;
					getline(ifs, str);
					string kk = str.substr(str.find(":", 0) + 1, str.find(",", 0) - str.find(":", 0) - 2);
					kk.erase(std::remove(kk.begin(), kk.end(), '\"'), kk.end());
					I.Set_Tmax(stoi(kk));
//					string kk = str.substr(str.find(":", 0) + 1, str.find(",", 0) - str.find(":", 0) - 2);
//					I.Set_Tmax(stoi(str.substr(str.find(":", 0) + 2, str.find(",", 0) - str.find(":", 0) - 3)));
//					string kk = str.substr(str.find("\"", 0) , str.find(",", 0) - str.find("\"", 0) - 2);
//					I.Set_Tmax(stoi(str.substr(str.find("\"", 0) + 1, str.find(",", 0) - str.find("\"", 0) +1)));
					getline(ifs, str);//ahora lee los deltas
					vector<int> deltas(M_T);
					int temp_val;
					for (int i = 0; i < M_T; i++)
					{
						getline(ifs, str);
						size_t pos = str.find(",");
						temp_val = stoi(str.substr(1, pos - 1));
						deltas[i]=(temp_val);
					}
					I.Set_Delta(deltas);
					I.Set_Min_Delta(*min_element(I.Get_Delta().begin(), I.Get_Delta().end()));
					getline(ifs, str);//ahora lee los workload
					getline(ifs, str);
					//Empieza los workload
					vector<int> workload_res;
					map<int, map<int, map<int, double >>>  workload;
					do
					{
						getline(ifs, str);
						//Empieza un recurso
						if (str.find(":{", 0) != str.npos || str.find(": {", 0) != str.npos)
						{
							if (str.find(":{", 0) != str.npos)
								name = str.substr(str.find("\"", 0) + 1, str.find(":{", 0) - str.find("\"", 0) - 2);
							else
								name = str.substr(str.find("\"", 0) + 1, str.find(": {", 0) - str.find("\"", 0) - 2);
	

							string resource = name;
							map<int, map<int, double >> work_res;


							do
							{
								getline(ifs, str);
								//Empieza una season
								if (str.find(":{", 0) != str.npos || str.find(": {", 0) != str.npos)
								{
									if (str.find(":{", 0) != str.npos)
										name = str.substr(str.find("\"", 0) + 1, str.find(":{", 0) - str.find("\"", 0) - 2);
									else
										name = str.substr(str.find("\"", 0) + 1, str.find(": {", 0) - str.find("\"", 0) - 2);


									string tactual = name;
									map<int, double >  work_sea;

									do
									{
										getline(ifs, str);
										if (str.find("}", 0) == str.npos)
										{

											string tpast = str.substr(str.find("\"", 0) + 1, str.find(":", 0) - str.find("\"", 0) - 2);
											string val_str = str.substr(str.find(":", 0) + 1, str.find(",", 0) - str.find(":", 0) - 1);
											double val = stod(str.substr(str.find(":", 0) + 1, str.find(",", 0) - str.find(":", 0) - 1));

											work_sea.insert(pair<int, double >(atoi(tpast.c_str()) - 1, val));



										}

									} while (str.find("}") == str.npos);
									work_res.insert(pair<int, map<int, double >>(atoi(tactual.c_str()) - 1, work_sea));


								}

							} while (str.find("},") != str.npos);
							getline(ifs, str);
							workload.insert(pair<int, map<int, map<int, double >>>(M_R_Id[resource], work_res));
							workload_res.push_back(M_R_Id[resource]);
						}
						
						

					}while (str.find("},") != str.npos);
//					vector<vector<vector<double>>> workload_N;

					vector<vector<vector<double>>> workload_N(M_N_Resources);


					//Poner en vector
					for (size_t i = 0; i < M_N_Resources; i++)
					{
						vector<vector<double> > workload_N2;
						if (find(workload_res.begin(),workload_res.end(),i) != workload_res.end())
						{
//							vector<vector<double> > workload_N2(I.Get_Tmax());
							for (int t = 0; t < I.Get_Tmax(); t++)
							{
								vector<double> workload_N3(I.Get_Delta(t));
								int cont = 0;
								for (int t2 = t; t2 < t + I.Get_Delta(t); t2++)
								{

									if ((workload.find(i) != workload.end())
										&& (workload[i].find(t2) != workload[i].end())
										&& (workload[i][t2].find(t) != workload[i][t2].end()))

										//			workload_N3.push_back(workload[i][t2][t]);
										//		else
										//			workload_N3.push_back(0);
										workload_N3[cont] = (workload[i][t2][t]);
									else
										workload_N3[cont] = 0;
									cont++;

								}
								//		workload_N2.push_back(workload_N3);
								workload_N2.push_back(workload_N3);
							}
						}
						//		workload_N.push_back(workload_N2);
						workload_N[i] = (workload_N2);

					}
					I.Set_Workload_Res(workload_res);
//					I.Set_Workload(workload);
					I.Set_WorkloadN(workload_N);

					//Accumulative workload
					vector<double > vec_workloadC(M_T);

					for (map<int, map<int, map<int, double >>>::iterator itw = workload.begin(); itw != workload.end(); itw++)
					{

						for (map<int, map<int, double >>::iterator itw2 = itw->second.begin(); itw2 != itw->second.end(); itw2++)
						{

							for (map<int, double> ::iterator itw3 = itw2->second.begin(); itw3 != itw2->second.end(); itw3++)
							{

								vec_workloadC[itw3->first] += ((itw3->second)/(M_Resources[(*itw).first].Get_Upper()[(*itw2).first]));
							}
						}
					}
					//					for (int ik = 0; ik < M_T; ik++)
					vector <double> tempWxt(vec_workloadC.size());
					for (size_t i=0; i < vec_workloadC.size(); i++)
					{
						tempWxt[i] = vec_workloadC[i] / I.Get_Delta()[i];
					}
					I.Set_WorkloadCxT(tempWxt);
					I.Set_WorkloadC(vec_workloadC);
					I.Set_Min_WorkloadxT(tempWxt[Min(tempWxt)]);
					I.Set_Max_WorkloadxT(tempWxt[Max(tempWxt)]);
					I.Set_Min_Workload(vec_workloadC[Min(vec_workloadC)]);
					I.Set_Max_Workload(vec_workloadC[Max(vec_workloadC)]);

					//Empieza los risk
					//TODO
					getline(ifs, str);
					getline(ifs, str);
					map<int, map<int, vector< double > >> risk;
					do
					{
						getline(ifs, str);
						//Empieza un recurso
						if (str.find(":{", 0) != str.npos || str.find(": {", 0) != str.npos)
						{
							if (str.find(":{", 0) != str.npos)
								name = str.substr(str.find("\"", 0) + 1, str.find(":{", 0) - str.find("\"", 0) - 2);
							else
								name = str.substr(str.find("\"", 0) + 1, str.find(": {", 0) - str.find("\"", 0) - 2);

							string tactual = name;

							
							map<int, vector< double > > risk_actual;

							do
							{
								
								getline(ifs, str);
								//Empieza una season
								if (str.find(":[", 0) != str.npos || str.find(": [", 0) != str.npos)
								{
									if (str.find(":[", 0) != str.npos)
										name = str.substr(str.find("\"", 0) + 1, str.find(":[", 0) - str.find("\"", 0) - 2);
									else
										name = str.substr(str.find("\"", 0) + 1, str.find(": [", 0) - str.find("\"", 0) - 2);

									string tinicio = name;
									
									double temp2;
									vector<double> risk_scenarios;
									do
									{
										getline(ifs, str);
										size_t pos = str.find(",");
										temp2 = stod(str.substr(1, pos - 1));
										risk_scenarios.push_back(temp2);
									} while (str.find(",") != str.npos);

									risk_actual.insert(pair<int,vector< double> >(atoi(tinicio.c_str()) - 1,risk_scenarios));

									getline(ifs, str);
								}

							} while (str.find("]") != str.npos);

							risk.insert(pair<int, map<int, vector<double>  >>(atoi(tactual.c_str()) - 1,risk_actual));

	

						}



					} while (str.find("},") != str.npos);
					vector<vector<vector<double>>> riskN;
//					vector<vector<double>> risk_actualN;
//					vector<double> risk_scenarios;

					//Riesgo acumulado por día de inicio
					vector<double > vec_riskC(M_T);
					vector<double > vec_riskCxT(M_T);
					vector<double> vec_riskQuantile(M_T);
					vector<double> vec_riskExcess(M_T);
					vector<vector<double>> mat_riskexcess;
					//			vector<double> vec_riskMC;

					//Poner en vector
					for (int ta = 0; ta < I.Get_Tmax(); ta++)
					{
						vector<vector<double>> risk_actualN;
						vector<double> vec_riskexcess(deltas[ta]);
						for (int ta2 =ta; ta2 <ta+deltas[ta]; ta2++)
						{

							vector<double> risk_scenarios(M_Scenarios_number[ta2]);
							double acumulado = 0;
							vector<double> temp(M_Scenarios_number[ta2]);
							for (int t2 = 0; t2 < M_Scenarios_number[ta2]; t2++)
							{
								if ((risk.find(ta2) != risk.end())
									&& (risk[ta2].find(ta) != risk[ta2].end()))
									risk_scenarios[t2]=(risk[ta2][ta][t2]);// [ta2] [ta] [t2] );
								else
									risk_scenarios[t2]=(0);

//									risk_scenarios.push_back(risk[ta2][ta][t2]);// [ta2] [ta] [t2] );
//								else
//									risk_scenarios.push_back(0);
								acumulado += risk_scenarios[t2];
								//
								//temp.push_back(risk_scenarios[t2]);
								temp[t2] = risk_scenarios[t2];
							}
							double media = acumulado / M_Scenarios_number[ta2];
							vec_riskC[ta] += acumulado / M_Scenarios_number[ta2];
							if (M_Scenarios_number[ta2] > 1)
							{
								int pos = ceil(M_Scenarios_number[ta2] * M_Quantile) - 1;


								nth_element(temp.begin(), temp.begin() + pos, temp.end());
								//		double cuantil1= M_T_Temp_Risk_S[i][pos];
								//		 sort(M_T_Temp_Risk_S[i].begin(), M_T_Temp_Risk_S[i].end());
								vec_riskQuantile[ta] += temp[pos];
								vec_riskExcess[ta] += max((double)0, (temp[pos] - media));
								vec_riskexcess[ta2-ta] = max((double)0, (temp[pos] - media)); 
								if (vec_riskexcess[ta2 - ta] < -1*__FLT_EPSILON__)
									Problema(111);
							}
							else
							{
								vec_riskQuantile[ta] = vec_riskC[ta];
								vec_riskExcess[ta] = 0;
								vec_riskexcess[ta2-ta] = 0;
							}
							risk_actualN.push_back(risk_scenarios);

						}
						mat_riskexcess.push_back(vec_riskexcess);
						riskN.push_back(risk_actualN);


					}
//					I.Set_Workload(workload);
					I.Set_RiskExcessXt(mat_riskexcess);
					//Quito para ahorrar memoria
//					I.Set_Risk(risk);
					I.Set_RiskN(riskN);

					for (size_t i=0; i < vec_riskCxT.size(); i++)
					{
						vec_riskCxT[i] = vec_riskC[i] /  I.Get_Delta()[i];
					}
					I.Set_RiskExcess(vec_riskExcess);
					I.Set_RiskC(vec_riskC);
					I.Set_RiskCxT(vec_riskCxT);
					I.Set_RiskCQuantile(vec_riskQuantile);
					I.Set_Min_RiskxT(vec_riskCxT[Min(vec_riskCxT)]);
					I.Set_Max_RiskxT(vec_riskCxT[Max(vec_riskCxT)]);

					I.Set_Max_Risk(vec_riskC[Max(vec_riskC)]);
					I.Set_Max_RiskQ(vec_riskQuantile[Max(vec_riskQuantile)]);
					vector<int> T_ordenados_Risk;
					vector<int> T_ordenados_RiskQ;
					//En T ordenados deben estar todos los que entran
					//hay algunos que entran solos
					for (int i1 = 0; i1 < (I.Get_Tmax()); i1++)
					{
						if (PuedoPonerSola(I,i1) == false) continue;
						bool colocado1 = false;
						for (int i2 = 0; i2 < T_ordenados_Risk.size() && colocado1 == false; i2++)
						{
							if (vec_riskC[i1] < (vec_riskC[T_ordenados_Risk[i2]] - __FLT_EPSILON__))
							{
								T_ordenados_Risk.insert(T_ordenados_Risk.begin() + i2, i1);
								colocado1 = true;
							}
						}

						if (colocado1== false)
							T_ordenados_Risk.push_back(i1);
						bool colocado2 = false;
						for (int i2 = 0; i2 < T_ordenados_RiskQ.size() && colocado2 == false; i2++)
						{
							if (vec_riskQuantile[i1] < (vec_riskQuantile[T_ordenados_RiskQ[i2]] - __FLT_EPSILON__))
							{
								T_ordenados_RiskQ.insert(T_ordenados_RiskQ.begin() + i2, i1);
								colocado2 = true;
							}
						}

						if (colocado2 == false)
							T_ordenados_RiskQ.push_back(i1);
					}
					I.Set_Min_Risk(vec_riskC[T_ordenados_Risk[0]]);
					I.Set_Min_RiskQ(vec_riskQuantile[T_ordenados_RiskQ[0]]);
					if (T_ordenados_Risk.size() > 1)
						I.Set_Regret(vec_riskC[T_ordenados_Risk[1]] - vec_riskC[T_ordenados_Risk[0]]);
					else
						I.Set_Regret(0);
					if (T_ordenados_RiskQ.size() > 1)
						I.Set_RegretQ(vec_riskQuantile[T_ordenados_RiskQ[1]] - vec_riskQuantile[T_ordenados_RiskQ[0]]);
					else
						I.Set_RegretQ(0);
//					I.Set_Max_Risk(vec_riskC[T_ordenados_Risk[T_ordenados_Risk.size()-1]]);
					double val1 = vec_riskC[T_ordenados_Risk[0]];
					list<int> T_ordenados_Risk_I;
					for (int i = 1; i < T_ordenados_Risk.size(); i++)
					{
						if (vec_riskC[T_ordenados_Risk[i]] > (val1 + __FLT_EPSILON__))
						{
							val1 = vec_riskC[T_ordenados_Risk[i]];
							T_ordenados_Risk_I.push_back(i);
						}
					}
					T_ordenados_Risk_I.push_back(T_ordenados_Risk.size());
					val1 = vec_riskQuantile[T_ordenados_RiskQ[0]];
					list<int> T_ordenados_Risk_IQ;
					for (int i = 1; i < T_ordenados_RiskQ.size(); i++)
					{
						if (vec_riskQuantile[T_ordenados_RiskQ[i]] > (val1 + __FLT_EPSILON__))
						{
							val1 = vec_riskQuantile[T_ordenados_RiskQ[i]];
							T_ordenados_Risk_IQ.push_back(i);
						}
					}
					T_ordenados_Risk_IQ.push_back(T_ordenados_RiskQ.size());
					I.Set_T_ordenados_Risk_IQ(T_ordenados_Risk_IQ);
					I.Set_T_ordenados_RiskQ(T_ordenados_RiskQ);
					I.Set_T_ordenados_Risk_I(T_ordenados_Risk_I);
					I.Set_T_ordenados_Risk(T_ordenados_Risk);
					M_N_Interventions =cont;
					M_Interventions.push_back(I);
//					cont++;


				}
				getline(ifs, str);
				getline(ifs, str);
			} while (str.find("},") != str.npos && (str.find("}") != str.npos));

		}
	}
/*		if (str.find("Seasons", index) != str.npos)
		{
			int kk = 9;
		}
		if (str.find("Interventions", index) != str.npos)
		{
			int kk = 9;
		}
		*/
	//Ahora Exclusions
		//leidos resources
//	int kk = 9;
	if (bexclusions == false)
	{
		//	ifs.clear();
//	ifs.seekg(0, ifs.beg);
		while (getline(ifs, str) && bexclusions == false)
		{
			size_t index = 0;
			if (str.find("Exclusions", index) != str.npos)
			{
				bexclusions = true;

				do
				{
					getline(ifs, str);
					//Empieza una season
					if (str.find(":[", 0) != str.npos || str.find(": [", 0) != str.npos)
					{
						vector <int> exclusion_int;
						vector <string> exclusion;
						getline(ifs, str);
						name = str.substr(str.find("\"", 0) + 1, str.find("\"", str.find("\"", 0) + 1) - str.find("\"", 0) - 1);

						exclusion.push_back(name);
						getline(ifs, str);
						string name2 = str.substr(str.find("\"", 0) + 1, str.find("\"", str.find("\"", 0) + 1) - str.find("\"", 0) - 1);
						exclusion.push_back(name2);
						getline(ifs, str);
						string name3 = str.substr(str.find("\"", 0) + 1, str.find("\"", str.find("\"", 0) + 1) - str.find("\"", 0) - 1);
						exclusion.push_back(name3);

						M_Exclusions_Originals.push_back(exclusion);
						exclusion_int.push_back(M_I_Id[exclusion[0]]);
						exclusion_int.push_back(M_I_Id[exclusion[1]]);
						exclusion_int.push_back(M_S_Id[exclusion[2]]);
						M_Interventions[M_I_Id[exclusion[0]]].Add_Exclusion(M_I_Id[exclusion[1]], M_S_Id[exclusion[2]]);
						M_Interventions[M_I_Id[exclusion[1]]].Add_Exclusion(M_I_Id[exclusion[0]], M_S_Id[exclusion[2]]);
						//Convert string to int
						M_Exclusions.push_back(exclusion_int);
						M_N_Exclusions++;
					}
					getline(ifs, str);
				} while (str.find("],") != str.npos);

			}
		}
	}


	/*
	else
	{
		for (list<vector<string>>::iterator it = M_Exclusions_Originals.begin(); it != M_Exclusions_Originals.end(); it++)
		{
			vector <int> exclusion_int;
			exclusion_int.push_back(M_I_Id[(*it)[0]]);
			exclusion_int.push_back(M_I_Id[(*it)[1]]);
			exclusion_int.push_back(M_S_Id[(*it)[2]]);
			M_Interventions[M_I_Id[(*it)[0]]].Add_Exclusion(M_I_Id[(*it)[1]], M_S_Id[(*it)[2]]);
			M_Interventions[M_I_Id[(*it)[1]]].Add_Exclusion(M_I_Id[(*it)[0]], M_S_Id[(*it)[2]]);
			//Convert string to int
			M_Exclusions.push_back(exclusion_int);
		}
	}*/
/*		while (true) {

			index = str.find(":{", index);
			if (index == string::npos) break;
			str.replace(index, 3, 1, '-');
			index += 1;

		}*/

	/*
	json j = json::parse(ifs);
	//T
	if (j.find("T") != j.end()) {
		json info = j["T"];
		M_T = info;

	}
	//Quantile
	if (j.find("Quantile") != j.end()) {
		json info = j["Quantile"];
		M_Quantile = info;
	}
	//Alpha
	if (j.find("Alpha") != j.end()) {
		json info = j["Alpha"];
		M_Alpha = info;
	}
	cout << M_T << M_Quantile << M_Alpha;
	M_T_Season.clear();
	for (int i = 0; i < M_T; i++)
		M_T_Season.push_back(-1);
	// Seasons

	if (j.find("Scenarios_number") != j.end()) {

		M_Scenarios_number = j["Scenarios_number"].get<vector<int>>();
	}

	if (j.find("Seasons") != j.end())
	{
		json info = j["Seasons"];
		size_t i=0;
		for (json::iterator it = info.begin(); it != info.end(); ++it)
		{
			M_S_Id.insert(pair<string, int>(it.key(), i));
			M_S_Name.push_back(it.key());
			Season S(i);

			S.Set_Name(it.key());

			if ((*it).is_number())
				S.Set_Numbers((*it).get<vector<int>>());
			else
			{
				vector <string> kk = (*it);
				vector < int> intNumbers;
				for (int ik = 0; ik < kk.size(); ik++)
				{
					int num = atoi(kk.at(ik).c_str());
					intNumbers.push_back(num);
				}
				S.Set_Numbers(intNumbers);
			}
			for (vector<int>::iterator it2 = S.Get_Numbers().begin(); it2 != S.Get_Numbers().end(); it2++)
			{
				M_T_Season[(*it2) - 1] = i;
			}
			M_Seasons.push_back(S);
			i++;
		}
	}
	// Resources


	if (j.find("Resources") != j.end())
	{
		json info = j["Resources"];

		// verify that "name" attribute exists in info
		size_t i=0;
		for (json::iterator it = info.begin(); it != info.end(); ++it)
		{
			//           string kk = it.key().substr(11, it.key().size() - 11);
					   //            exclusion_int.push_back(atoi(kk.c_str()));
			Resource R(i);
			R.Set_Name(it.key());
			vector<double> vec(M_T);
			R.Set_Temp(vec);
			M_R_Name.push_back(it.key());
			M_R_Id.insert(pair<string, int>(it.key(), i));

			if ((*it).find("max") != (*it).end())
			{
				R.Set_Upper((*it)["max"].get<vector<double>>());
				//              cout << (*it)["max"]; // upper

			}
			if ((*it).find("min") != (*it).end())
			{
				R.Set_Lower((*it)["min"].get<vector<double>>());
				//                cout << (*it)["min"]; // upper

			}
			M_Resources.push_back(R);
			M_N_Resources++;
			i++;



		}
	}

	// Interventions

	if (j.find("Interventions") != j.end())
	{
		json info = j["Interventions"];
		int sizei = info.size();
		cout << "número de interventions" << sizei << endl;
		// verify that "name" attribute exists in info
		size_t i=0;
		for (json::iterator it = info.begin(); it != info.end(); ++it)
		{
			Intervention I(i);
			I.Set_Name(it.key());
			M_I_Id.insert(pair<string, int>(it.key(), i));
			M_I_Name.push_back(it.key());
			//           string kk = it.key().substr(13, it.key().size() - 13);
		   //            exclusion_int.push_back(atoi(kk.c_str()));


			if ((*it).find("tmax") != (*it).end())
			{
				if ((*it)["tmax"].is_number())
				{
					I.Set_Tmax((*it)["tmax"] - 1);
				}
				else
				{
					string  kk = (*it)["tmax"];
					I.Set_Tmax(atoi(kk.c_str()) - 1);
				}

				//               cout << I.t_max; // upper


			}
			if ((*it).find("Delta") != (*it).end())
			{
				I.Set_Delta((*it)["Delta"].get<vector<int>>());
				//              cout << (*it)["Delta"]; // upper
				I.Set_Min_Delta(*min_element(I.Get_Delta().begin(), I.Get_Delta().end()));

			}
			if ((*it).find("workload") != (*it).end())
			{
				map<int, map<int, map<int, double >>>  workload;
				//           vector<int> workload_r;
				json info3 = (*it)["workload"];
				for (json::iterator it2 = info3.begin(); it2 != info3.end(); ++it2)
				{
					//                 workload_r.push_back(M_R_Id[it2.key()]);
					map<int, map<int, double >> work_res;
					//Resource
					json info4 = info3[it2.key()];
					for (json::iterator it3 = info4.begin(); it3 != info4.end(); ++it3)
					{
						//Time  current  
						map<int, double >  work_sea;
						json info5 = info4[it3.key()];
						for (json::iterator it4 = info5.begin(); it4 != info5.end(); ++it4)
						{
							//Time ini
							json info6 = info5[it4.key()];
							work_sea.insert(pair<int, double >(atoi(it4.key().c_str()) - 1, info6));



						}
						work_res.insert(pair<int, map<int, double >>(atoi(it3.key().c_str()) - 1, work_sea));

					}
					workload.insert(pair<int, map<int, map<int, double >>>(M_R_Id[it2.key()], work_res));
					I.Set_Workload(workload);
					//                   I.Set_WorkloadR(workload_r);


									//   I.Set_WorkloadT(workload_r);

				}

				//Accumulative workload
				vector<double > vec_workloadC;
				for (int ik = 0; ik < M_T; ik++)
				{
					vec_workloadC.push_back(0);

				}
				for (map<int, map<int, map<int, double >>>::iterator itw = workload.begin(); itw != workload.end(); itw++)
				{

					for (map<int, map<int, double >>::iterator itw2 = itw->second.begin(); itw2 != itw->second.end(); itw2++)
					{

						for (map<int, double> ::iterator itw3 = itw2->second.begin(); itw3 != itw2->second.end(); itw3++)
						{

							vec_workloadC[itw3->first] += itw3->second;
						}
					}
				}

				I.Set_WorkloadC(vec_workloadC);
				I.Set_Min_Workload(vec_workloadC[Min(vec_workloadC)]);
				I.Set_Max_Workload(vec_workloadC[Max(vec_workloadC)]);
			}
			if ((*it).find("risk") != (*it).end())
			{
				map<int, map<int, vector< double > >> risk;
				json info3 = (*it)["risk"];
				for (json::iterator it2 = info3.begin(); it2 != info3.end(); ++it2)
				{
					//SEason
					map<int, vector< double > > risk_sea;
					json info4 = info3[it2.key()];
					for (json::iterator it3 = info4.begin(); it3 != info4.end(); ++it3)
					{

						//Time
						json info5 = info4[it3.key()];
						//Scenarios
						vector<double> temp = info5.get<vector<double>>();

						temp.push_back(accumulate(temp.begin(), temp.end(), 0.0));

						risk_sea.insert(pair<int, vector<double >>(atoi(it3.key().c_str()) - 1, temp));

					}
					risk.insert(pair<int, map<int, vector<double >>>(atoi(it2.key().c_str()) - 1, risk_sea));
				}
				I.Set_Risk(risk);

				//Riesgo acumulado por día de inicio
				vector<double > vec_riskC;
				for (int ik = 0; ik < M_T; ik++)
				{
					vec_riskC.push_back(0);

				}
				for (map<int, map<int, vector< double > >>::iterator itm = risk.begin(); itm != risk.end(); itm++)
				{


					for (map<int, vector< double > >::iterator it2m = itm->second.begin(); it2m != itm->second.end(); it2m++)
					{
						vec_riskC[it2m->first] += it2m->second[it2m->second.size() - 1];
					}

				}
				I.Set_RiskC(vec_riskC);
				I.Set_Min_Risk(vec_riskC[Min(vec_riskC)]);
				I.Set_Max_Risk(vec_riskC[Max(vec_riskC)]);
				vector<int> T_ordenados_Risk;
				for (int i1 = 0; i1 < (I.Get_Tmax()); i1++)
				{
					bool colocado = false;
					for (int i2 = 0; i2 < T_ordenados_Risk.size() && colocado == false; i2++)
					{
						if (vec_riskC[i1] < (vec_riskC[T_ordenados_Risk[i2]] - __FLT_EPSILON__))
						{
							T_ordenados_Risk.insert(T_ordenados_Risk.begin() + i2, i1);
							colocado = true;
						}
					}
					if (colocado == false)
						T_ordenados_Risk.push_back(i1);
				}
				I.Set_T_ordenados_Risk(T_ordenados_Risk);



			}
			M_N_Interventions = i;
			M_Interventions.push_back(I);
			i++;
		}
	}




	//Exclusion
	if (j.find("Exclusions") != j.end())
	{
		json info = j["Exclusions"];
		int sizei = info.size();
		//      cout << sizei << endl;
		for (json::iterator it = info.begin(); it != info.end(); ++it)
		{
			vector <int> exclusion_int;
			vector <string> exclusion = info[it.key()];

			M_Exclusions_Originals.push_back(exclusion);
			exclusion_int.push_back(M_I_Id[exclusion[0]]);
			exclusion_int.push_back(M_I_Id[exclusion[1]]);
			exclusion_int.push_back(M_S_Id[exclusion[2]]);
			M_Interventions[M_I_Id[exclusion[0]]].Add_Exclusion(M_I_Id[exclusion[1]], M_S_Id[exclusion[2]]);
			M_Interventions[M_I_Id[exclusion[1]]].Add_Exclusion(M_I_Id[exclusion[0]], M_S_Id[exclusion[2]]);
			//Convert string to int
			M_Exclusions.push_back(exclusion_int);
			M_N_Exclusions++;
		}

	}
	*/

}

void Maintenance::leer_problem_wojsonFast(string& name_file)
{
	// open the json file - here replaced with a istringstream containing the json data


	struct timeb t1;
	M_Name_file = name_file;
	string name_file2 = name_file + ".json";
	ifstream ifs(name_file2);
	if (!ifs.is_open()) {

		cout << "El fichero " << name_file2 << "está abierto" << endl;
		return;
	}


	ftime(&t1);
	M_Time_Initial = t1;
	//Otra manera de leer un archivo tan grande
	char* buffer1;
//	size_t buffer1;

//	std::ifstream fin("input.dat");
	bool T = false;
	bool A = false;
	bool C = false;
	bool S = false;
	bool Q = false;

//Otra manera de leer un archivo tan grande
		stringstream bufferN;

			bufferN << ifs.rdbuf();


		// Do whatever you need with first count bytes in the buffer
		// ...

		size_t index = 0;
		streampos posT = bufferN.str().find("\"T\"", index);
		if (posT != bufferN.str().npos)
		{
			size_t kk = bufferN.str().find(":", posT);
			size_t pos = bufferN.str().find(",", posT);
			string kk3 = bufferN.str().substr(kk + 1, pos - kk - 1);
			M_T = stoi(kk3);
			T = true;

		}
		size_t  posQ = bufferN.str().find("Quantile", index);
		if (posQ != bufferN.str().npos)
		{
			size_t kk = bufferN.str().find(":", posQ);
			size_t pos = bufferN.str().find(",", posQ);
			string kk3 = bufferN.str().substr(kk + 1, pos - kk - 1);
			M_Quantile = stod(kk3);
			Q = true;
		}

		size_t  posA = bufferN.str().find("Alpha", index);
		if (posA != bufferN.str().npos)
		{
			size_t kk = bufferN.str().find(":", posA);
			size_t pos = bufferN.str().find(",", posA);
			string kk3 = bufferN.str().substr(kk + 1, pos - kk - 1);
			M_Alpha = stod(kk3);
			A = true;
		}
		size_t  posC = bufferN.str().find("ComputationTime", index);
		if (posC != bufferN.str().npos)
		{
			size_t kk = bufferN.str().find(":", posC);
			size_t pos = bufferN.str().find(",", posC);
			string kk3 = bufferN.str().substr(kk + 1, pos - kk - 1);
			M_ComputionTime = stoi(kk3);
			C = true;
		}
		size_t  posS = bufferN.str().find("Scenarios_number", index);
		if (posS != bufferN.str().npos)
		{
			int temp;
			M_Total_Scenarios = 0;
			size_t pos = bufferN.str().find(",", posS);
			size_t pos_b = pos - 5;
			for (int i = 0; i < M_T; i++)
			{


				temp = stoi(bufferN.str().substr(pos_b, pos - 1));
				pos_b = pos;
				if (temp > 1)
					M_Pool = true;
				M_Total_Scenarios += temp;
				M_Scenarios_number.push_back(temp);
				if (i < (M_T - 2))
				{
					pos = bufferN.str().find(",", pos + 1);
					pos_b = pos - 5;
				}
				else
				{
					pos_b = pos + 1;
					pos = bufferN.str().find("]", pos + 1);
				}

			}
			S = true;


		}


	string str;
	if (!Q || !S || !T || !A || !C)
	{
		//Primero leemos valores
		ifs.clear();
		ifs.seekg(0, ifs.beg);

		while (getline(ifs, str))
		{
			size_t index = 0;
			if (str.find("\"T\"", index) != str.npos)
			{
				size_t kk = str.find(":");
				size_t pos = str.find(",");
				string kk3 = str.substr(kk + 1, pos - kk - 1);
				M_T = stoi(kk3);

			}
			if (str.find("Quantile", index) != str.npos)
			{
				size_t kk = str.find(":");
				size_t pos = str.find(",");
				string kk3 = str.substr(kk + 1, pos - kk - 1);
				M_Quantile = stod(kk3);
			}


			if (str.find("Alpha", index) != str.npos)
			{
				size_t kk = str.find(":");
				size_t pos = str.find(",");
				string kk3 = str.substr(kk + 1, pos - kk - 1);
				M_Alpha = stod(kk3);
			}
			if (str.find("ComputationTime", index) != str.npos)
			{
				size_t kk = str.find(":");
				size_t pos = str.find(",");
				string kk3 = str.substr(kk + 1, pos - kk - 1);
				M_ComputionTime = stoi(kk3);
			}
			if (str.find("Scenarios_number", index) != str.npos)
			{
				int temp;
				M_Total_Scenarios = 0;
				for (int i = 0; i < M_T; i++)
				{
					getline(ifs, str);
					size_t pos = str.find(",");
					temp = stoi(str.substr(1, pos - 1));
					if (temp > 1)
						M_Pool = true;
					M_Total_Scenarios += temp;
					M_Scenarios_number.push_back(temp);

				}


			}
		}
	}
	for (int i = 0; i < M_T; i++)
	{
		M_T_Season.push_back(-1);
	}
	//	settingsFile.seekg(0, settingsFile.beg);
	ifs.clear();
	ifs.seekg(0, ifs.beg);
	string name;
	//Seasons

	bool bSeason = false;
	//Resources
	bool bResources = false;
	bool bexclusions = false;
	while (getline(ifs, str) && (bSeason == false || bResources == false))
	{
		size_t index = 0;

		if (str.find("Seasons\":{", index) != str.npos || str.find("Seasons\":", index) != str.npos)
		{
			bSeason = true;
			int cont = 0;
			do
			{
				getline(ifs, str);
				//Empieza una season
				if (str.find(":[", 0) != str.npos || str.find(": [", 0) != str.npos)
				{
					if (str.find(":[", 0) != str.npos)
						name = str.substr(str.find("\"", 0) + 1, str.find(":[", 0) - str.find("\"", 0) - 2);
					else
						name = str.substr(str.find("\"", 0) + 1, str.find(": [", 0) - str.find("\"", 0) - 2);
					M_S_Id.insert(pair<string, int>(name, cont));
					M_S_Name.push_back(name);
					Season S(cont);

					S.Set_Name(name);
					vector<int> numbers;
					do
					{

						getline(ifs, str);
						if (str.size() != 0)
						{

							if (str.find("],") == str.npos && (str.find("]") == str.npos))
							{
								name = str.substr(str.find("\"", 0) + 1, str.find(",", 0) - str.find("\"", 0) - 2);
								numbers.push_back(stoi(name));
							}
						}

					} while (str.find("],") == str.npos && (str.find("]") == str.npos));
					S.Set_Numbers(numbers);

					for (vector<int>::iterator it2 = S.Get_Numbers().begin(); it2 != S.Get_Numbers().end(); it2++)
					{
						M_T_Season[(*it2) - 1] = cont;
						//						M_T_Season[(*it2) ] = cont;

					}
					M_Seasons.push_back(S);
					cont++;

				}
			} while (str.find("],") != str.npos && (str.find("]") != str.npos));
		}
		index = 0;

		if (str.find("Resources", index) != str.npos)
		{
			bResources = true;
			int cont = 0;
			do
			{
				getline(ifs, str);
				//Empieza una season
				if (str.find(":{", 0) != str.npos || str.find(": {", 0) != str.npos)
				{
					if (str.find(":{", 0) != str.npos)
						name = str.substr(str.find("\"", 0) + 1, str.find(":{", 0) - str.find("\"", 0) - 2);
					else
						name = str.substr(str.find("\"", 0) + 1, str.find(": {", 0) - str.find("\"", 0) - 2);



					Resource R(cont);
					R.Set_Name(name);
					vector<double> vec(M_T);
					R.Set_Temp(vec);
					R.Set_Copy(vec);
					M_R_Name.push_back(name);
					M_R_Id.insert(pair<string, int>(name, cont));

					vector<double> min;
					vector<double> max;
					getline(ifs, str); //lee min
					double temp;
					bool abajo = false;
					for (int i = 0; i < M_T; i++)
					{
						getline(ifs, str);
						size_t pos = str.find(",");
						temp = stod(str.substr(1, pos - 1));
						if (temp > __FLT_EPSILON__)
						{
							M_Lower_Resources = true;
							abajo = true;
						}
						min.push_back(temp);

					}
					getline(ifs, str); //lee el fin de la anterior
					getline(ifs, str); //lee max
					for (int i = 0; i < M_T; i++)
					{
						getline(ifs, str);
						size_t pos = str.find(",");
						temp = stod(str.substr(1, pos - 1));
						max.push_back(temp);

					}
					R.Set_Lower(min);
					R.Set_Upper(max);

					M_Lower_ResourcesV.push_back(abajo);
					M_Resources.push_back(R);
					M_N_Resources++;
					cont++;



				}
				getline(ifs, str); //lee el ]
				getline(ifs, str);
			} while (str.find("},") != str.npos);
		}



	}
	if (bResources == false)
	{
		ifs.clear();
		ifs.seekg(0, ifs.beg);
		while (getline(ifs, str) && (bResources == false))
		{

			size_t index = 0;
			if (str.find("Resources", index) != str.npos)
			{
				bResources = true;
				int cont = 0;
				do
				{
					getline(ifs, str);
					//Empieza una season
					if (str.find(":{", 0) != str.npos || str.find(": {", 0) != str.npos)
					{
						if (str.find(":{", 0) != str.npos)
							name = str.substr(str.find("\"", 0) + 1, str.find(":{", 0) - str.find("\"", 0) - 2);
						else
							name = str.substr(str.find("\"", 0) + 1, str.find(": {", 0) - str.find("\"", 0) - 2);



						Resource R(cont);
						R.Set_Name(name);
						vector<double> vec(M_T);
						R.Set_Temp(vec);
						R.Set_Copy(vec);
						M_R_Name.push_back(name);
						M_R_Id.insert(pair<string, int>(name, cont));

						vector<double> min;
						vector<double> max;
						getline(ifs, str); //lee min
						double temp;
						bool abajo = false;
						for (int i = 0; i < M_T; i++)
						{
							getline(ifs, str);
							size_t pos = str.find(",");
							temp = stod(str.substr(1, pos - 1));
							if (temp > __FLT_EPSILON__)
							{
								M_Lower_Resources = true;
								abajo = true;
							}
							min.push_back(temp);

						}
						getline(ifs, str); //lee el fin de la anterior
						getline(ifs, str); //lee max
						for (int i = 0; i < M_T; i++)
						{
							getline(ifs, str);
							size_t pos = str.find(",");
							temp = stod(str.substr(1, pos - 1));
							max.push_back(temp);

						}
						R.Set_Lower(min);
						R.Set_Upper(max);

						M_Lower_ResourcesV.push_back(abajo);
						M_Resources.push_back(R);
						M_N_Resources++;
						cont++;



					}
					getline(ifs, str); //lee el ]
					getline(ifs, str);
				} while (str.find("},") != str.npos);
			}
		}

	}
	if (bSeason == false)
	{
		ifs.clear();
		ifs.seekg(0, ifs.beg);
		while (getline(ifs, str) && (bSeason == false))
		{
			size_t index = 0;

			if (str.find("Seasons\":{", index) != str.npos || str.find("Seasons\":", index) != str.npos)
			{
				bSeason = true;
				int cont = 0;
				do
				{
					getline(ifs, str);
					//Empieza una season
					if (str.find(":[", 0) != str.npos || str.find(": [", 0) != str.npos)
					{
						if (str.find(":[", 0) != str.npos)
							name = str.substr(str.find("\"", 0) + 1, str.find(":[", 0) - str.find("\"", 0) - 2);
						else
							name = str.substr(str.find("\"", 0) + 1, str.find(": [", 0) - str.find("\"", 0) - 2);
						M_S_Id.insert(pair<string, int>(name, cont));
						M_S_Name.push_back(name);
						Season S(cont);

						S.Set_Name(name);
						vector<int> numbers;
						do
						{

							getline(ifs, str);
							if (str.size() != 0)
							{

								if (str.find("],") == str.npos && (str.find("]") == str.npos))
								{
									name = str.substr(str.find("\"", 0) + 1, str.find(",", 0) - str.find("\"", 0) - 2);
									numbers.push_back(stoi(name));
								}
							}

						} while (str.find("],") == str.npos && (str.find("]") == str.npos));
						S.Set_Numbers(numbers);

						for (vector<int>::iterator it2 = S.Get_Numbers().begin(); it2 != S.Get_Numbers().end(); it2++)
						{
							M_T_Season[(*it2) - 1] = cont;
							//						M_T_Season[(*it2) ] = cont;

						}
						M_Seasons.push_back(S);
						cont++;

					}
				} while (str.find("],") != str.npos && (str.find("]") != str.npos));
			}
			index = 0;

		}

	}


	//leidos resources
//	ifs.clear();
//	ifs.seekg(0, ifs.beg);
	bool bInterventions = false;
	while (getline(ifs, str) && (bInterventions == false))
	{
		size_t index = 0;

		if (str.find("Interventions", index) != str.npos)
		{
			bInterventions = true;
			int cont = 0;
			do
			{
				getline(ifs, str);
				//Empieza una intervention
				if (str.find(":{", 0) != str.npos || str.find(": {", 0) != str.npos)
				{
					if (str.find(":{", 0) != str.npos)
						name = str.substr(str.find("\"", 0) + 1, str.find(":{", 0) - str.find("\"", 0) - 2);
					else
						name = str.substr(str.find("\"", 0) + 1, str.find(": {", 0) - str.find("\"", 0) - 2);
					Intervention I(cont);
					I.Set_Name(name);
					M_I_Id.insert(pair<string, int>(name, cont));
					M_I_Name.push_back(name);
					cont++;
					getline(ifs, str);
					string kk = str.substr(str.find(":", 0) + 1, str.find(",", 0) - str.find(":", 0) - 2);
					kk.erase(std::remove(kk.begin(), kk.end(), '\"'), kk.end());
					I.Set_Tmax(stoi(kk));
					//					string kk = str.substr(str.find(":", 0) + 1, str.find(",", 0) - str.find(":", 0) - 2);
					//					I.Set_Tmax(stoi(str.substr(str.find(":", 0) + 2, str.find(",", 0) - str.find(":", 0) - 3)));
					//					string kk = str.substr(str.find("\"", 0) , str.find(",", 0) - str.find("\"", 0) - 2);
					//					I.Set_Tmax(stoi(str.substr(str.find("\"", 0) + 1, str.find(",", 0) - str.find("\"", 0) +1)));
					getline(ifs, str);//ahora lee los deltas
					vector<int> deltas(M_T);
					int temp_val;
					for (int i = 0; i < M_T; i++)
					{
						getline(ifs, str);
						size_t pos = str.find(",");
						temp_val = stoi(str.substr(1, pos - 1));
						deltas[i] = (temp_val);
					}
					I.Set_Delta(deltas);
					I.Set_Min_Delta(*min_element(I.Get_Delta().begin(), I.Get_Delta().end()));
					getline(ifs, str);//ahora lee los workload
					getline(ifs, str);
					//Empieza los workload
					vector<int> workload_res;
					map<int, map<int, map<int, double >>>  workload;
					do
					{
						getline(ifs, str);
						//Empieza un recurso
						if (str.find(":{", 0) != str.npos || str.find(": {", 0) != str.npos)
						{
							if (str.find(":{", 0) != str.npos)
								name = str.substr(str.find("\"", 0) + 1, str.find(":{", 0) - str.find("\"", 0) - 2);
							else
								name = str.substr(str.find("\"", 0) + 1, str.find(": {", 0) - str.find("\"", 0) - 2);


							string resource = name;
							map<int, map<int, double >> work_res;


							do
							{
								getline(ifs, str);
								//Empieza una season
								if (str.find(":{", 0) != str.npos || str.find(": {", 0) != str.npos)
								{
									if (str.find(":{", 0) != str.npos)
										name = str.substr(str.find("\"", 0) + 1, str.find(":{", 0) - str.find("\"", 0) - 2);
									else
										name = str.substr(str.find("\"", 0) + 1, str.find(": {", 0) - str.find("\"", 0) - 2);


									string tactual = name;
									map<int, double >  work_sea;

									do
									{
										getline(ifs, str);
										if (str.find("}", 0) == str.npos)
										{

											string tpast = str.substr(str.find("\"", 0) + 1, str.find(":", 0) - str.find("\"", 0) - 2);
											string val_str = str.substr(str.find(":", 0) + 1, str.find(",", 0) - str.find(":", 0) - 1);
											double val = stod(str.substr(str.find(":", 0) + 1, str.find(",", 0) - str.find(":", 0) - 1));

											work_sea.insert(pair<int, double >(atoi(tpast.c_str()) - 1, val));



										}

									} while (str.find("}") == str.npos);
									work_res.insert(pair<int, map<int, double >>(atoi(tactual.c_str()) - 1, work_sea));


								}

							} while (str.find("},") != str.npos);
							getline(ifs, str);
							workload.insert(pair<int, map<int, map<int, double >>>(M_R_Id[resource], work_res));
							workload_res.push_back(M_R_Id[resource]);
						}



					} while (str.find("},") != str.npos);
					//					vector<vector<vector<double>>> workload_N;

					vector<vector<vector<double>>> workload_N(M_N_Resources);


					//Poner en vector
					for (size_t i = 0; i < M_N_Resources; i++)
					{
						vector<vector<double> > workload_N2;
						if (find(workload_res.begin(), workload_res.end(), i) != workload_res.end())
						{
							//							vector<vector<double> > workload_N2(I.Get_Tmax());
							for (int t = 0; t < I.Get_Tmax(); t++)
							{
								vector<double> workload_N3(I.Get_Delta(t));
								int cont = 0;
								for (int t2 = t; t2 < t + I.Get_Delta(t); t2++)
								{

									if ((workload.find(i) != workload.end())
										&& (workload[i].find(t2) != workload[i].end())
										&& (workload[i][t2].find(t) != workload[i][t2].end()))

										//			workload_N3.push_back(workload[i][t2][t]);
										//		else
										//			workload_N3.push_back(0);
										workload_N3[cont] = (workload[i][t2][t]);
									else
										workload_N3[cont] = 0;
									cont++;

								}
								//		workload_N2.push_back(workload_N3);
								workload_N2.push_back(workload_N3);
							}
						}
						//		workload_N.push_back(workload_N2);
						workload_N[i] = (workload_N2);

					}
					I.Set_Workload_Res(workload_res);
					//					I.Set_Workload(workload);
					I.Set_WorkloadN(workload_N);

					//Accumulative workload
					vector<double > vec_workloadC(M_T);

					for (map<int, map<int, map<int, double >>>::iterator itw = workload.begin(); itw != workload.end(); itw++)
					{

						for (map<int, map<int, double >>::iterator itw2 = itw->second.begin(); itw2 != itw->second.end(); itw2++)
						{

							for (map<int, double> ::iterator itw3 = itw2->second.begin(); itw3 != itw2->second.end(); itw3++)
							{

								vec_workloadC[itw3->first] += ((itw3->second) / (M_Resources[(*itw).first].Get_Upper()[(*itw2).first]));
							}
						}
					}
					//					for (int ik = 0; ik < M_T; ik++)
					vector <double> tempWxt(vec_workloadC.size());
					for (size_t i = 0; i < vec_workloadC.size(); i++)
					{
						tempWxt[i] = vec_workloadC[i] / I.Get_Delta()[i];
					}
					I.Set_WorkloadCxT(tempWxt);
					I.Set_WorkloadC(vec_workloadC);
					I.Set_Min_WorkloadxT(tempWxt[Min(tempWxt)]);
					I.Set_Max_WorkloadxT(tempWxt[Max(tempWxt)]);
					I.Set_Min_Workload(vec_workloadC[Min(vec_workloadC)]);
					I.Set_Max_Workload(vec_workloadC[Max(vec_workloadC)]);

					//Empieza los risk
					//TODO
					getline(ifs, str);
					getline(ifs, str);
					map<int, map<int, vector< double > >> risk;
					do
					{
						getline(ifs, str);
						//Empieza un recurso
						if (str.find(":{", 0) != str.npos || str.find(": {", 0) != str.npos)
						{
							if (str.find(":{", 0) != str.npos)
								name = str.substr(str.find("\"", 0) + 1, str.find(":{", 0) - str.find("\"", 0) - 2);
							else
								name = str.substr(str.find("\"", 0) + 1, str.find(": {", 0) - str.find("\"", 0) - 2);

							string tactual = name;


							map<int, vector< double > > risk_actual;

							do
							{

								getline(ifs, str);
								//Empieza una season
								if (str.find(":[", 0) != str.npos || str.find(": [", 0) != str.npos)
								{
									if (str.find(":[", 0) != str.npos)
										name = str.substr(str.find("\"", 0) + 1, str.find(":[", 0) - str.find("\"", 0) - 2);
									else
										name = str.substr(str.find("\"", 0) + 1, str.find(": [", 0) - str.find("\"", 0) - 2);

									string tinicio = name;

									double temp2;
									vector<double> risk_scenarios;
									do
									{
										getline(ifs, str);
										size_t pos = str.find(",");
										temp2 = stod(str.substr(1, pos - 1));
										risk_scenarios.push_back(temp2);
									} while (str.find(",") != str.npos);

									risk_actual.insert(pair<int, vector< double> >(atoi(tinicio.c_str()) - 1, risk_scenarios));

									getline(ifs, str);
								}

							} while (str.find("]") != str.npos);

							risk.insert(pair<int, map<int, vector<double>  >>(atoi(tactual.c_str()) - 1, risk_actual));



						}



					} while (str.find("},") != str.npos);
					vector<vector<vector<double>>> riskN;
					//					vector<vector<double>> risk_actualN;
					//					vector<double> risk_scenarios;

										//Riesgo acumulado por día de inicio
					vector<double > vec_riskC(M_T);
					vector<double > vec_riskCxT(M_T);
					vector<double> vec_riskQuantile(M_T);
					vector<double> vec_riskExcess(M_T);
					vector<vector<double>> mat_riskexcess;
					//			vector<double> vec_riskMC;

					//Poner en vector
					for (int ta = 0; ta < I.Get_Tmax(); ta++)
					{
						vector<vector<double>> risk_actualN;
						vector<double> vec_riskexcess(deltas[ta]);
						for (int ta2 = ta; ta2 < ta + deltas[ta]; ta2++)
						{

							vector<double> risk_scenarios(M_Scenarios_number[ta2]);
							double acumulado = 0;
							vector<double> temp(M_Scenarios_number[ta2]);
							for (int t2 = 0; t2 < M_Scenarios_number[ta2]; t2++)
							{
								if ((risk.find(ta2) != risk.end())
									&& (risk[ta2].find(ta) != risk[ta2].end()))
									risk_scenarios[t2] = (risk[ta2][ta][t2]);// [ta2] [ta] [t2] );
								else
									risk_scenarios[t2] = (0);

								//									risk_scenarios.push_back(risk[ta2][ta][t2]);// [ta2] [ta] [t2] );
								//								else
								//									risk_scenarios.push_back(0);
								acumulado += risk_scenarios[t2];
								//
								//temp.push_back(risk_scenarios[t2]);
								temp[t2] = risk_scenarios[t2];
							}
							double media = acumulado / M_Scenarios_number[ta2];
							vec_riskC[ta] += acumulado / M_Scenarios_number[ta2];
							if (M_Scenarios_number[ta2] > 1)
							{
								int pos = ceil(M_Scenarios_number[ta2] * M_Quantile) - 1;


								nth_element(temp.begin(), temp.begin() + pos, temp.end());
								//		double cuantil1= M_T_Temp_Risk_S[i][pos];
								//		 sort(M_T_Temp_Risk_S[i].begin(), M_T_Temp_Risk_S[i].end());
								vec_riskQuantile[ta] += temp[pos];
								vec_riskExcess[ta] += max((double)0, (temp[pos] - media));
								vec_riskexcess[ta2 - ta] = max((double)0, (temp[pos] - media));
								if (vec_riskexcess[ta2 - ta] < -1 * __FLT_EPSILON__)
									Problema(111);
							}
							else
							{
								vec_riskQuantile[ta] = vec_riskC[ta];
								vec_riskExcess[ta] = 0;
								vec_riskexcess[ta2 - ta] = 0;
							}
							risk_actualN.push_back(risk_scenarios);

						}
						mat_riskexcess.push_back(vec_riskexcess);
						riskN.push_back(risk_actualN);


					}
					//					I.Set_Workload(workload);
					I.Set_RiskExcessXt(mat_riskexcess);
					//Quito para ahorrar memoria
//					I.Set_Risk(risk);
					I.Set_RiskN(riskN);

					for (size_t i = 0; i < vec_riskCxT.size(); i++)
					{
						vec_riskCxT[i] = vec_riskC[i] / I.Get_Delta()[i];
					}
					I.Set_RiskExcess(vec_riskExcess);
					I.Set_RiskC(vec_riskC);
					I.Set_RiskCxT(vec_riskCxT);
					I.Set_RiskCQuantile(vec_riskQuantile);
					I.Set_Min_RiskxT(vec_riskCxT[Min(vec_riskCxT)]);
					I.Set_Max_RiskxT(vec_riskCxT[Max(vec_riskCxT)]);

					I.Set_Max_Risk(vec_riskC[Max(vec_riskC)]);
					I.Set_Max_RiskQ(vec_riskQuantile[Max(vec_riskQuantile)]);
					vector<int> T_ordenados_Risk;
					vector<int> T_ordenados_RiskQ;
					//En T ordenados deben estar todos los que entran
					//hay algunos que entran solos
					for (int i1 = 0; i1 < (I.Get_Tmax()); i1++)
					{
						if (PuedoPonerSola(I, i1) == false) continue;
						bool colocado1 = false;
						for (int i2 = 0; i2 < T_ordenados_Risk.size() && colocado1 == false; i2++)
						{
							if (vec_riskC[i1] < (vec_riskC[T_ordenados_Risk[i2]] - __FLT_EPSILON__))
							{
								T_ordenados_Risk.insert(T_ordenados_Risk.begin() + i2, i1);
								colocado1 = true;
							}
						}

						if (colocado1 == false)
							T_ordenados_Risk.push_back(i1);
						bool colocado2 = false;
						for (int i2 = 0; i2 < T_ordenados_RiskQ.size() && colocado2 == false; i2++)
						{
							if (vec_riskQuantile[i1] < (vec_riskQuantile[T_ordenados_RiskQ[i2]] - __FLT_EPSILON__))
							{
								T_ordenados_RiskQ.insert(T_ordenados_RiskQ.begin() + i2, i1);
								colocado2 = true;
							}
						}

						if (colocado2 == false)
							T_ordenados_RiskQ.push_back(i1);
					}
					I.Set_Min_Risk(vec_riskC[T_ordenados_Risk[0]]);
					I.Set_Min_RiskQ(vec_riskQuantile[T_ordenados_RiskQ[0]]);
					if (T_ordenados_Risk.size() > 1)
						I.Set_Regret(vec_riskC[T_ordenados_Risk[1]] - vec_riskC[T_ordenados_Risk[0]]);
					else
						I.Set_Regret(0);
					if (T_ordenados_RiskQ.size() > 1)
						I.Set_RegretQ(vec_riskQuantile[T_ordenados_RiskQ[1]] - vec_riskQuantile[T_ordenados_RiskQ[0]]);
					else
						I.Set_RegretQ(0);
					//					I.Set_Max_Risk(vec_riskC[T_ordenados_Risk[T_ordenados_Risk.size()-1]]);
					double val1 = vec_riskC[T_ordenados_Risk[0]];
					list<int> T_ordenados_Risk_I;
					for (int i = 1; i < T_ordenados_Risk.size(); i++)
					{
						if (vec_riskC[T_ordenados_Risk[i]] > (val1 + __FLT_EPSILON__))
						{
							val1 = vec_riskC[T_ordenados_Risk[i]];
							T_ordenados_Risk_I.push_back(i);
						}
					}
					T_ordenados_Risk_I.push_back(T_ordenados_Risk.size());
					val1 = vec_riskQuantile[T_ordenados_RiskQ[0]];
					list<int> T_ordenados_Risk_IQ;
					for (int i = 1; i < T_ordenados_RiskQ.size(); i++)
					{
						if (vec_riskQuantile[T_ordenados_RiskQ[i]] > (val1 + __FLT_EPSILON__))
						{
							val1 = vec_riskQuantile[T_ordenados_RiskQ[i]];
							T_ordenados_Risk_IQ.push_back(i);
						}
					}
					T_ordenados_Risk_IQ.push_back(T_ordenados_RiskQ.size());
					I.Set_T_ordenados_Risk_IQ(T_ordenados_Risk_IQ);
					I.Set_T_ordenados_RiskQ(T_ordenados_RiskQ);
					I.Set_T_ordenados_Risk_I(T_ordenados_Risk_I);
					I.Set_T_ordenados_Risk(T_ordenados_Risk);
					M_N_Interventions = cont;
					M_Interventions.push_back(I);
					//					cont++;


				}
				getline(ifs, str);
				getline(ifs, str);
			} while (str.find("},") != str.npos && (str.find("}") != str.npos));

		}
	}
	/*		if (str.find("Seasons", index) != str.npos)
			{
				int kk = 9;
			}
			if (str.find("Interventions", index) != str.npos)
			{
				int kk = 9;
			}
			*/
			//Ahora Exclusions
				//leidos resources

	if (bexclusions == false)
	{
		//	ifs.clear();
//	ifs.seekg(0, ifs.beg);
		while (getline(ifs, str) && bexclusions == false)
		{
			size_t index = 0;
			if (str.find("Exclusions", index) != str.npos)
			{
				bexclusions = true;

				do
				{
					getline(ifs, str);
					//Empieza una season
					if (str.find(":[", 0) != str.npos || str.find(": [", 0) != str.npos)
					{
						vector <int> exclusion_int;
						vector <string> exclusion;
						getline(ifs, str);
						name = str.substr(str.find("\"", 0) + 1, str.find("\"", str.find("\"", 0) + 1) - str.find("\"", 0) - 1);

						exclusion.push_back(name);
						getline(ifs, str);
						string name2 = str.substr(str.find("\"", 0) + 1, str.find("\"", str.find("\"", 0) + 1) - str.find("\"", 0) - 1);
						exclusion.push_back(name2);
						getline(ifs, str);
						string name3 = str.substr(str.find("\"", 0) + 1, str.find("\"", str.find("\"", 0) + 1) - str.find("\"", 0) - 1);
						exclusion.push_back(name3);

						M_Exclusions_Originals.push_back(exclusion);
						exclusion_int.push_back(M_I_Id[exclusion[0]]);
						exclusion_int.push_back(M_I_Id[exclusion[1]]);
						exclusion_int.push_back(M_S_Id[exclusion[2]]);
						M_Interventions[M_I_Id[exclusion[0]]].Add_Exclusion(M_I_Id[exclusion[1]], M_S_Id[exclusion[2]]);
						M_Interventions[M_I_Id[exclusion[1]]].Add_Exclusion(M_I_Id[exclusion[0]], M_S_Id[exclusion[2]]);
						//Convert string to int
						M_Exclusions.push_back(exclusion_int);
						M_N_Exclusions++;
					}
					getline(ifs, str);
				} while (str.find("],") != str.npos);

			}
		}
	}
	if (bexclusions == false)
	{
		ifs.clear();
		ifs.seekg(0, ifs.beg);
		while (getline(ifs, str) && bexclusions == false)
		{
			size_t index = 0;
			if (str.find("Exclusions", index) != str.npos)
			{
				bexclusions = true;

				do
				{
					getline(ifs, str);
					//Empieza una season
					if (str.find(":[", 0) != str.npos || str.find(": [", 0) != str.npos)
					{
						vector <int> exclusion_int;
						vector <string> exclusion;
						getline(ifs, str);
						name = str.substr(str.find("\"", 0) + 1, str.find("\"", str.find("\"", 0) + 1) - str.find("\"", 0) - 1);

						exclusion.push_back(name);
						getline(ifs, str);
						string name2 = str.substr(str.find("\"", 0) + 1, str.find("\"", str.find("\"", 0) + 1) - str.find("\"", 0) - 1);
						exclusion.push_back(name2);
						getline(ifs, str);
						string name3 = str.substr(str.find("\"", 0) + 1, str.find("\"", str.find("\"", 0) + 1) - str.find("\"", 0) - 1);
						exclusion.push_back(name3);

						M_Exclusions_Originals.push_back(exclusion);
						exclusion_int.push_back(M_I_Id[exclusion[0]]);
						exclusion_int.push_back(M_I_Id[exclusion[1]]);
						exclusion_int.push_back(M_S_Id[exclusion[2]]);
						M_Interventions[M_I_Id[exclusion[0]]].Add_Exclusion(M_I_Id[exclusion[1]], M_S_Id[exclusion[2]]);
						M_Interventions[M_I_Id[exclusion[1]]].Add_Exclusion(M_I_Id[exclusion[0]], M_S_Id[exclusion[2]]);
						//Convert string to int
						M_Exclusions.push_back(exclusion_int);
						M_N_Exclusions++;
					}
					getline(ifs, str);
				} while (str.find("],") != str.npos);

			}
		}
	}
	/*
	else
	{
		for (list<vector<string>>::iterator it = M_Exclusions_Originals.begin(); it != M_Exclusions_Originals.end(); it++)
		{
			vector <int> exclusion_int;
			exclusion_int.push_back(M_I_Id[(*it)[0]]);
			exclusion_int.push_back(M_I_Id[(*it)[1]]);
			exclusion_int.push_back(M_S_Id[(*it)[2]]);
			M_Interventions[M_I_Id[(*it)[0]]].Add_Exclusion(M_I_Id[(*it)[1]], M_S_Id[(*it)[2]]);
			M_Interventions[M_I_Id[(*it)[1]]].Add_Exclusion(M_I_Id[(*it)[0]], M_S_Id[(*it)[2]]);
			//Convert string to int
			M_Exclusions.push_back(exclusion_int);
		}
	}*/
	/*		while (true) {

				index = str.find(":{", index);
				if (index == string::npos) break;
				str.replace(index, 3, 1, '-');
				index += 1;

			}*/

			/*
			json j = json::parse(ifs);
			//T
			if (j.find("T") != j.end()) {
				json info = j["T"];
				M_T = info;

			}
			//Quantile
			if (j.find("Quantile") != j.end()) {
				json info = j["Quantile"];
				M_Quantile = info;
			}
			//Alpha
			if (j.find("Alpha") != j.end()) {
				json info = j["Alpha"];
				M_Alpha = info;
			}
			cout << M_T << M_Quantile << M_Alpha;
			M_T_Season.clear();
			for (int i = 0; i < M_T; i++)
				M_T_Season.push_back(-1);
			// Seasons

			if (j.find("Scenarios_number") != j.end()) {

				M_Scenarios_number = j["Scenarios_number"].get<vector<int>>();
			}

			if (j.find("Seasons") != j.end())
			{
				json info = j["Seasons"];
				size_t i=0;
				for (json::iterator it = info.begin(); it != info.end(); ++it)
				{
					M_S_Id.insert(pair<string, int>(it.key(), i));
					M_S_Name.push_back(it.key());
					Season S(i);

					S.Set_Name(it.key());

					if ((*it).is_number())
						S.Set_Numbers((*it).get<vector<int>>());
					else
					{
						vector <string> kk = (*it);
						vector < int> intNumbers;
						for (int ik = 0; ik < kk.size(); ik++)
						{
							int num = atoi(kk.at(ik).c_str());
							intNumbers.push_back(num);
						}
						S.Set_Numbers(intNumbers);
					}
					for (vector<int>::iterator it2 = S.Get_Numbers().begin(); it2 != S.Get_Numbers().end(); it2++)
					{
						M_T_Season[(*it2) - 1] = i;
					}
					M_Seasons.push_back(S);
					i++;
				}
			}
			// Resources


			if (j.find("Resources") != j.end())
			{
				json info = j["Resources"];

				// verify that "name" attribute exists in info
				size_t i=0;
				for (json::iterator it = info.begin(); it != info.end(); ++it)
				{
					//           string kk = it.key().substr(11, it.key().size() - 11);
							   //            exclusion_int.push_back(atoi(kk.c_str()));
					Resource R(i);
					R.Set_Name(it.key());
					vector<double> vec(M_T);
					R.Set_Temp(vec);
					M_R_Name.push_back(it.key());
					M_R_Id.insert(pair<string, int>(it.key(), i));

					if ((*it).find("max") != (*it).end())
					{
						R.Set_Upper((*it)["max"].get<vector<double>>());
						//              cout << (*it)["max"]; // upper

					}
					if ((*it).find("min") != (*it).end())
					{
						R.Set_Lower((*it)["min"].get<vector<double>>());
						//                cout << (*it)["min"]; // upper

					}
					M_Resources.push_back(R);
					M_N_Resources++;
					i++;



				}
			}

			// Interventions

			if (j.find("Interventions") != j.end())
			{
				json info = j["Interventions"];
				int sizei = info.size();
				cout << "número de interventions" << sizei << endl;
				// verify that "name" attribute exists in info
				size_t i=0;
				for (json::iterator it = info.begin(); it != info.end(); ++it)
				{
					Intervention I(i);
					I.Set_Name(it.key());
					M_I_Id.insert(pair<string, int>(it.key(), i));
					M_I_Name.push_back(it.key());
					//           string kk = it.key().substr(13, it.key().size() - 13);
				   //            exclusion_int.push_back(atoi(kk.c_str()));


					if ((*it).find("tmax") != (*it).end())
					{
						if ((*it)["tmax"].is_number())
						{
							I.Set_Tmax((*it)["tmax"] - 1);
						}
						else
						{
							string  kk = (*it)["tmax"];
							I.Set_Tmax(atoi(kk.c_str()) - 1);
						}

						//               cout << I.t_max; // upper


					}
					if ((*it).find("Delta") != (*it).end())
					{
						I.Set_Delta((*it)["Delta"].get<vector<int>>());
						//              cout << (*it)["Delta"]; // upper
						I.Set_Min_Delta(*min_element(I.Get_Delta().begin(), I.Get_Delta().end()));

					}
					if ((*it).find("workload") != (*it).end())
					{
						map<int, map<int, map<int, double >>>  workload;
						//           vector<int> workload_r;
						json info3 = (*it)["workload"];
						for (json::iterator it2 = info3.begin(); it2 != info3.end(); ++it2)
						{
							//                 workload_r.push_back(M_R_Id[it2.key()]);
							map<int, map<int, double >> work_res;
							//Resource
							json info4 = info3[it2.key()];
							for (json::iterator it3 = info4.begin(); it3 != info4.end(); ++it3)
							{
								//Time  current
								map<int, double >  work_sea;
								json info5 = info4[it3.key()];
								for (json::iterator it4 = info5.begin(); it4 != info5.end(); ++it4)
								{
									//Time ini
									json info6 = info5[it4.key()];
									work_sea.insert(pair<int, double >(atoi(it4.key().c_str()) - 1, info6));



								}
								work_res.insert(pair<int, map<int, double >>(atoi(it3.key().c_str()) - 1, work_sea));

							}
							workload.insert(pair<int, map<int, map<int, double >>>(M_R_Id[it2.key()], work_res));
							I.Set_Workload(workload);
							//                   I.Set_WorkloadR(workload_r);


											//   I.Set_WorkloadT(workload_r);

						}

						//Accumulative workload
						vector<double > vec_workloadC;
						for (int ik = 0; ik < M_T; ik++)
						{
							vec_workloadC.push_back(0);

						}
						for (map<int, map<int, map<int, double >>>::iterator itw = workload.begin(); itw != workload.end(); itw++)
						{

							for (map<int, map<int, double >>::iterator itw2 = itw->second.begin(); itw2 != itw->second.end(); itw2++)
							{

								for (map<int, double> ::iterator itw3 = itw2->second.begin(); itw3 != itw2->second.end(); itw3++)
								{

									vec_workloadC[itw3->first] += itw3->second;
								}
							}
						}

						I.Set_WorkloadC(vec_workloadC);
						I.Set_Min_Workload(vec_workloadC[Min(vec_workloadC)]);
						I.Set_Max_Workload(vec_workloadC[Max(vec_workloadC)]);
					}
					if ((*it).find("risk") != (*it).end())
					{
						map<int, map<int, vector< double > >> risk;
						json info3 = (*it)["risk"];
						for (json::iterator it2 = info3.begin(); it2 != info3.end(); ++it2)
						{
							//SEason
							map<int, vector< double > > risk_sea;
							json info4 = info3[it2.key()];
							for (json::iterator it3 = info4.begin(); it3 != info4.end(); ++it3)
							{

								//Time
								json info5 = info4[it3.key()];
								//Scenarios
								vector<double> temp = info5.get<vector<double>>();

								temp.push_back(accumulate(temp.begin(), temp.end(), 0.0));

								risk_sea.insert(pair<int, vector<double >>(atoi(it3.key().c_str()) - 1, temp));

							}
							risk.insert(pair<int, map<int, vector<double >>>(atoi(it2.key().c_str()) - 1, risk_sea));
						}
						I.Set_Risk(risk);

						//Riesgo acumulado por día de inicio
						vector<double > vec_riskC;
						for (int ik = 0; ik < M_T; ik++)
						{
							vec_riskC.push_back(0);

						}
						for (map<int, map<int, vector< double > >>::iterator itm = risk.begin(); itm != risk.end(); itm++)
						{


							for (map<int, vector< double > >::iterator it2m = itm->second.begin(); it2m != itm->second.end(); it2m++)
							{
								vec_riskC[it2m->first] += it2m->second[it2m->second.size() - 1];
							}

						}
						I.Set_RiskC(vec_riskC);
						I.Set_Min_Risk(vec_riskC[Min(vec_riskC)]);
						I.Set_Max_Risk(vec_riskC[Max(vec_riskC)]);
						vector<int> T_ordenados_Risk;
						for (int i1 = 0; i1 < (I.Get_Tmax()); i1++)
						{
							bool colocado = false;
							for (int i2 = 0; i2 < T_ordenados_Risk.size() && colocado == false; i2++)
							{
								if (vec_riskC[i1] < (vec_riskC[T_ordenados_Risk[i2]] - __FLT_EPSILON__))
								{
									T_ordenados_Risk.insert(T_ordenados_Risk.begin() + i2, i1);
									colocado = true;
								}
							}
							if (colocado == false)
								T_ordenados_Risk.push_back(i1);
						}
						I.Set_T_ordenados_Risk(T_ordenados_Risk);



					}
					M_N_Interventions = i;
					M_Interventions.push_back(I);
					i++;
				}
			}




			//Exclusion
			if (j.find("Exclusions") != j.end())
			{
				json info = j["Exclusions"];
				int sizei = info.size();
				//      cout << sizei << endl;
				for (json::iterator it = info.begin(); it != info.end(); ++it)
				{
					vector <int> exclusion_int;
					vector <string> exclusion = info[it.key()];

					M_Exclusions_Originals.push_back(exclusion);
					exclusion_int.push_back(M_I_Id[exclusion[0]]);
					exclusion_int.push_back(M_I_Id[exclusion[1]]);
					exclusion_int.push_back(M_S_Id[exclusion[2]]);
					M_Interventions[M_I_Id[exclusion[0]]].Add_Exclusion(M_I_Id[exclusion[1]], M_S_Id[exclusion[2]]);
					M_Interventions[M_I_Id[exclusion[1]]].Add_Exclusion(M_I_Id[exclusion[0]], M_S_Id[exclusion[2]]);
					//Convert string to int
					M_Exclusions.push_back(exclusion_int);
					M_N_Exclusions++;
				}

			}
			*/

}

void Maintenance::leer_problem_wojson2(string& name_file)
{
	// open the json file - here replaced with a istringstream containing the json data


	struct timeb t1;
	M_Name_file = name_file;
	string name_file2 = name_file + ".json";
	ifstream ifs(name_file2);
	if (!ifs.is_open()) {

		cout << "El fichero " << name_file2 << "está abierto" << endl;
		return ;
	}
	ftime(&t1);
	M_Time_Initial = t1;

	//Primero leemos valores

	string str;
	while (getline(ifs, str))
	{
		size_t index = 0;
		if (str.find("\"T\"", index) != str.npos)
		{
			size_t kk = str.find(":");
			size_t pos = str.find(",");
			string kk3 = str.substr(kk + 1, pos - kk - 1);
			M_T = stoi(kk3);

		}
		if (str.find("Quantile", index) != str.npos)
		{
			size_t kk = str.find(":");
			size_t pos = str.find(",");
			string kk3 = str.substr(kk + 1, pos - kk - 1);
			M_Quantile = stod(kk3);
		}


		if (str.find("Alpha", index) != str.npos)
		{
			size_t kk = str.find(":");
			size_t pos = str.find(",");
			string kk3 = str.substr(kk + 1, pos - kk - 1);
			M_Alpha = stod(kk3);
		}
		if (str.find("ComputationTime", index) != str.npos)
		{
			size_t kk = str.find(":");
			size_t pos = str.find(",");
			string kk3 = str.substr(kk + 1, pos - kk - 1);
			M_ComputionTime = stoi(kk3);
		}
		if (str.find("Scenarios_number", index) != str.npos)
		{
			int temp;
			M_Total_Scenarios = 0;
			for (size_t i = 0; i < M_T; i++)
			{
				getline(ifs, str);
				size_t pos = str.find(",");
				temp = stoi(str.substr(1, pos - 1));
				if (temp > 1)
					M_Pool = true;
				M_Total_Scenarios += temp;
				M_Scenarios_number.push_back(temp);

			}


		}


		if (str.find("Seasons\":{", index) != str.npos)
		{
			//bSeason = true;
			int cont = 0;
			do
			{
				getline(ifs, str);
				//Empieza una season
				if (str.find(":[", 0) != str.npos)
				{
					string name = str.substr(str.find("\"", 0) + 1, str.find(":[", 0) - str.find("\"", 0) - 2);
					M_S_Id.insert(pair<string, int>(name, cont));
					M_S_Name.push_back(name);
					Season S(cont);

					S.Set_Name(name);
					vector<int> numbers;
					do
					{

						getline(ifs, str);
						if (str.size() != 0)
						{

							if (str.find("],") == str.npos && (str.find("]") == str.npos))
							{
								name = str.substr(str.find("\"", 0) + 1, str.find(",", 0) - str.find("\"", 0) - 2);
								numbers.push_back(stoi(name));
							}
						}

					} while (str.find("],") == str.npos && (str.find("]") == str.npos));
					S.Set_Numbers(numbers);

					for (vector<int>::iterator it2 = S.Get_Numbers().begin(); it2 != S.Get_Numbers().end(); it2++)
					{
						M_T_Season[(*it2) - 1] = cont;
						//						M_T_Season[(*it2) ] = cont;

					}
					M_Seasons.push_back(S);
					cont++;

				}
			} while (str.find("],") != str.npos && (str.find("]") != str.npos));
		}


		if (str.find("Resources", index) != str.npos)
		{
	//		bResources = true;
			int cont = 0;
			do
			{
				getline(ifs, str);
				//Empieza una season
				if (str.find(":{", 0) != str.npos)
				{
					string name = str.substr(str.find("\"", 0) + 1, str.find(":{", 0) - str.find("\"", 0) - 2);

					Resource R(cont);
					R.Set_Name(name);
					vector<double> vec(M_T);
					R.Set_Temp(vec);
					R.Set_Copy(vec);
					M_R_Name.push_back(name);
					M_R_Id.insert(pair<string, int>(name, cont));

					vector<double> min;
					vector<double> max;
					getline(ifs, str); //lee min
					double temp;
					for (int i = 0; i < M_T; i++)
					{
						getline(ifs, str);
						size_t pos = str.find(",");
						temp = stod(str.substr(1, pos - 1));
						min.push_back(temp);

					}
					getline(ifs, str); //lee el fin de la anterior
					getline(ifs, str); //lee max
					for (int i = 0; i < M_T; i++)
					{
						getline(ifs, str);
						size_t pos = str.find(",");
						temp = stod(str.substr(1, pos - 1));
						max.push_back(temp);

					}
					R.Set_Lower(min);
					R.Set_Upper(max);


					M_Resources.push_back(R);
					M_N_Resources++;
					cont++;



				}
				getline(ifs, str); //lee el ]
				getline(ifs, str);
			} while (str.find("},") != str.npos);
		}



		if (str.find("Interventions", index) != str.npos)
		{
			int cont = 0;
			do
			{
				getline(ifs, str);
				//Empieza una intervention
				if (str.find(":{", 0) != str.npos)
				{
					string name = str.substr(str.find("\"", 0) + 1, str.find(":{", 0) - str.find("\"", 0) - 2);
					Intervention I(cont);
					I.Set_Name(name);
					M_I_Id.insert(pair<string, int>(name, cont));
					M_I_Name.push_back(name);
					cont++;
					getline(ifs, str);
					string kk = str.substr(str.find(":", 0) + 1, str.find(",", 0) - str.find(":", 0) - 2);
					I.Set_Tmax(stoi(str.substr(str.find(":", 0) + 2, str.find(",", 0) - str.find(":", 0) - 3)));
					getline(ifs, str);//ahora lee los deltas
					vector<int> deltas;
					int temp_val;
					for (int i = 0; i < M_T; i++)
					{
						getline(ifs, str);
						size_t pos = str.find(",");
						temp_val = stoi(str.substr(1, pos - 1));
						deltas.push_back(temp_val);
					}
					I.Set_Delta(deltas);
					I.Set_Min_Delta(*min_element(I.Get_Delta().begin(), I.Get_Delta().end()));
					getline(ifs, str);//ahora lee los workload
					getline(ifs, str);
					//Empieza los workload
					map<int, map<int, map<int, double >>>  workload;

					do
					{
						getline(ifs, str);
						//Empieza un recurso
						if (str.find(":{", 0) != str.npos)
						{

							string resource = str.substr(str.find("\"", 0) + 1, str.find(":{", 0) - str.find("\"", 0) - 2);
							map<int, map<int, double >> work_res;

							do
							{
								getline(ifs, str);
								//Empieza una season
								if (str.find(":{", 0) != str.npos)
								{


									string tactual = str.substr(str.find("\"", 0) + 1, str.find(":{", 0) - str.find("\"", 0) - 2);
									map<int, double >  work_sea;
									do
									{
										getline(ifs, str);
										if (str.find("}", 0) == str.npos)
										{

											string tpast = str.substr(str.find("\"", 0) + 1, str.find(":", 0) - str.find("\"", 0) - 2);
											string val_str = str.substr(str.find(":", 0) + 1, str.find(",", 0) - str.find(":", 0) - 1);
											double val = stod(str.substr(str.find(":", 0) + 1, str.find(",", 0) - str.find(":", 0) - 1));

											work_sea.insert(pair<int, double >(atoi(tpast.c_str()) - 1, val));


										}

									} while (str.find("}") == str.npos);
									work_res.insert(pair<int, map<int, double >>(atoi(tactual.c_str()) - 1, work_sea));


								}

							} while (str.find("},") != str.npos);
							getline(ifs, str);
							workload.insert(pair<int, map<int, map<int, double >>>(M_R_Id[resource], work_res));

						}



					} while (str.find("},") != str.npos);

					I.Set_Workload(workload);

					//Accumulative workload
					vector<double > vec_workloadC(M_T);

					for (map<int, map<int, map<int, double >>>::iterator itw = workload.begin(); itw != workload.end(); itw++)
					{

						for (map<int, map<int, double >>::iterator itw2 = itw->second.begin(); itw2 != itw->second.end(); itw2++)
						{

							for (map<int, double> ::iterator itw3 = itw2->second.begin(); itw3 != itw2->second.end(); itw3++)
							{

								vec_workloadC[itw3->first] += itw3->second;
							}
						}
					}
					//					for (int ik = 0; ik < M_T; ik++)
					vector <double> tempWxt(vec_workloadC.size());
					for (size_t i=0; i < vec_workloadC.size(); i++)
					{
						tempWxt[i] = vec_workloadC[i] / I.Get_Delta()[i];
					}
					I.Set_WorkloadCxT(tempWxt);
					I.Set_WorkloadC(vec_workloadC);
					I.Set_Min_WorkloadxT(tempWxt[Min(tempWxt)]);
					I.Set_Max_WorkloadxT(tempWxt[Max(tempWxt)]);
					I.Set_Min_Workload(vec_workloadC[Min(vec_workloadC)]);
					I.Set_Max_Workload(vec_workloadC[Max(vec_workloadC)]);

					//Empieza los risk
					//TODO
					getline(ifs, str);
					getline(ifs, str);
					map<int, map<int, vector< double > >> risk;
					do
					{
						getline(ifs, str);
						//Empieza un recurso
						if (str.find(":{", 0) != str.npos)
						{

							string tactual = str.substr(str.find("\"", 0) + 1, str.find(":{", 0) - str.find("\"", 0) - 2);


							map<int, vector< double > > risk_actual;
							do
							{

								getline(ifs, str);
								//Empieza una season
								if (str.find(":[", 0) != str.npos)
								{


									string tinicio = str.substr(str.find("\"", 0) + 1, str.find(":[", 0) - str.find("\"", 0) - 2);

									double temp;
									vector<double> risk_scenarios;
									do
									{
										getline(ifs, str);
										size_t pos = str.find(",");
										temp = stod(str.substr(1, pos - 1));
										risk_scenarios.push_back(temp);
									} while (str.find(",") != str.npos);

									risk_actual.insert(pair<int, vector< double> >(atoi(tinicio.c_str()) - 1, risk_scenarios));
									getline(ifs, str);
								}

							} while (str.find("]") != str.npos);

							risk.insert(pair<int, map<int, vector<double>  >>(atoi(tactual.c_str()) - 1, risk_actual));



						}



					} while (str.find("},") != str.npos);

					//					I.Set_Workload(workload);
					I.Set_Risk(risk);

					//Riesgo acumulado por día de inicio
					vector<double > vec_riskC(M_T);
					vector<double > vec_riskCxT(M_T);
					vector<double> vec_riskQuantile(M_T);
					//			vector<double> vec_riskMC;
//					for (int ik = 0; ik < M_T; ik++)
//					{
//						vec_riskC.push_back(0);
//						vec_riskCxT.push_back(0);
//						vec_riskQuantile.push_back(0);

//					}
					for (map<int, map<int, vector< double > >>::iterator itm = risk.begin(); itm != risk.end(); itm++)
					{


						for (map<int, vector< double > >::iterator it2m = itm->second.begin(); it2m != itm->second.end(); it2m++)
						{
							double acumulado = 0;
							vector<double> temp;
							//							temp.push_back(accumulate(temp.begin(), temp.end(), 0.0));
							for (int si = 0; si < it2m->second.size(); si++)
							{
								acumulado += it2m->second[si];
								temp.push_back(it2m->second[si]);
							}
							//		vec_riskC[it2m->first] += acumulado;
							vec_riskC[it2m->first] += acumulado / it2m->second.size();
							if (it2m->second.size() > 1)
							{
								int pos = ceil(it2m->second.size() * M_Quantile) - 1;


								nth_element(temp.begin(), temp.begin() + pos, temp.end());
								//		double cuantil1= M_T_Temp_Risk_S[i][pos];
								//		 sort(M_T_Temp_Risk_S[i].begin(), M_T_Temp_Risk_S[i].end());
								vec_riskQuantile[it2m->first] += temp[pos];
							}
							else
								vec_riskQuantile[it2m->first] = vec_riskC[it2m->first];

						}
						//						temp.push_back(accumulate(temp.begin(), temp.end(), 0.0));

					}
					for (size_t i=0; i < vec_riskCxT.size(); i++)
					{
						vec_riskCxT[i] = vec_riskC[i] / I.Get_Delta()[i];
					}
					I.Set_RiskC(vec_riskC);
					I.Set_RiskCxT(vec_riskCxT);
					I.Set_RiskCQuantile(vec_riskQuantile);
					I.Set_Min_RiskxT(vec_riskCxT[Min(vec_riskCxT)]);
					I.Set_Max_RiskxT(vec_riskCxT[Max(vec_riskCxT)]);

					I.Set_Max_Risk(vec_riskC[Max(vec_riskC)]);
					I.Set_Max_RiskQ(vec_riskQuantile[Max(vec_riskQuantile)]);
					vector<int> T_ordenados_Risk;
					vector<int> T_ordenados_RiskQ;
					//En T ordenados deben estar todos los que entran
					//hay algunos que entran solos
					for (int i1 = 0; i1 < (I.Get_Tmax()); i1++)
					{
						if (PuedoPonerSola(I, i1) == false) continue;
						bool colocado1 = false;
						for (int i2 = 0; i2 < T_ordenados_Risk.size() && colocado1 == false; i2++)
						{
							if (vec_riskC[i1] < (vec_riskC[T_ordenados_Risk[i2]] - __FLT_EPSILON__))
							{
								T_ordenados_Risk.insert(T_ordenados_Risk.begin() + i2, i1);
								colocado1 = true;
							}
						}

						if (colocado1 == false)
							T_ordenados_Risk.push_back(i1);
						bool colocado2 = false;
						for (int i2 = 0; i2 < T_ordenados_RiskQ.size() && colocado2 == false; i2++)
						{
							if (vec_riskQuantile[i1] < (vec_riskQuantile[T_ordenados_RiskQ[i2]] - __FLT_EPSILON__))
							{
								T_ordenados_RiskQ.insert(T_ordenados_RiskQ.begin() + i2, i1);
								colocado2 = true;
							}
						}

						if (colocado2 == false)
							T_ordenados_RiskQ.push_back(i1);
					}
					I.Set_Min_Risk(vec_riskC[T_ordenados_Risk[0]]);
					I.Set_Min_RiskQ(vec_riskQuantile[T_ordenados_RiskQ[0]]);
					if (T_ordenados_Risk.size() > 1)
						I.Set_Regret(vec_riskC[T_ordenados_Risk[1]] - vec_riskC[T_ordenados_Risk[0]]);
					else
						I.Set_Regret(0);
					if (T_ordenados_RiskQ.size() > 1)
						I.Set_RegretQ(vec_riskQuantile[T_ordenados_RiskQ[1]] - vec_riskQuantile[T_ordenados_RiskQ[0]]);
					else
						I.Set_RegretQ(0);
					//					I.Set_Max_Risk(vec_riskC[T_ordenados_Risk[T_ordenados_Risk.size()-1]]);
					double val1 = vec_riskC[T_ordenados_Risk[0]];
					list<int> T_ordenados_Risk_I;
					for (int i = 1; i < T_ordenados_Risk.size(); i++)
					{
						if (vec_riskC[T_ordenados_Risk[i]] > (val1 + __FLT_EPSILON__))
						{
							val1 = vec_riskC[T_ordenados_Risk[i]];
							T_ordenados_Risk_I.push_back(i);
						}
					}
					T_ordenados_Risk_I.push_back(T_ordenados_Risk.size());
					val1 = vec_riskQuantile[T_ordenados_RiskQ[0]];
					list<int> T_ordenados_Risk_IQ;
					for (int i = 1; i < T_ordenados_RiskQ.size(); i++)
					{
						if (vec_riskQuantile[T_ordenados_RiskQ[i]] > (val1 + __FLT_EPSILON__))
						{
							val1 = vec_riskQuantile[T_ordenados_RiskQ[i]];
							T_ordenados_Risk_IQ.push_back(i);
						}
					}
					T_ordenados_Risk_IQ.push_back(T_ordenados_RiskQ.size());
					I.Set_T_ordenados_Risk_IQ(T_ordenados_Risk_IQ);
					I.Set_T_ordenados_RiskQ(T_ordenados_RiskQ);
					I.Set_T_ordenados_Risk_I(T_ordenados_Risk_I);
					I.Set_T_ordenados_Risk(T_ordenados_Risk);
					M_N_Interventions = cont;
					M_Interventions.push_back(I);
					//					cont++;


				}
				getline(ifs, str);
				getline(ifs, str);
			} while (str.find("},") != str.npos && (str.find("}") != str.npos));

		}
		if (str.find("Exclusions", index) != str.npos)
		{
	//		bexclusions = true;

			do
			{
				getline(ifs, str);
				//Empieza una season
				if (str.find(":[", 0) != str.npos)
				{
					vector <int> exclusion_int;
					vector <string> exclusion;
					getline(ifs, str);
					string name = str.substr(str.find("\"", 0) + 1, str.find("\"", str.find("\"", 0) + 1) - str.find("\"", 0) - 1);

					exclusion.push_back(name);
					getline(ifs, str);
					string name2 = str.substr(str.find("\"", 0) + 1, str.find("\"", str.find("\"", 0) + 1) - str.find("\"", 0) - 1);
					exclusion.push_back(name2);
					getline(ifs, str);
					string name3 = str.substr(str.find("\"", 0) + 1, str.find("\"", str.find("\"", 0) + 1) - str.find("\"", 0) - 1);
					exclusion.push_back(name3);

					M_Exclusions_Originals.push_back(exclusion);
					exclusion_int.push_back(M_I_Id[exclusion[0]]);
					exclusion_int.push_back(M_I_Id[exclusion[1]]);
					exclusion_int.push_back(M_S_Id[exclusion[2]]);
					M_Interventions[M_I_Id[exclusion[0]]].Add_Exclusion(M_I_Id[exclusion[1]], M_S_Id[exclusion[2]]);
					M_Interventions[M_I_Id[exclusion[1]]].Add_Exclusion(M_I_Id[exclusion[0]], M_S_Id[exclusion[2]]);
					//Convert string to int
					M_Exclusions.push_back(exclusion_int);
					M_N_Exclusions++;
				}
				getline(ifs, str);
			} while (str.find("],") != str.npos);

		}
	}

}

bool Maintenance::VerifySolution()
{
	if (M_Challenge_Mode)
		return false;
	for (int j = 0; j < M_N_Interventions; j++)
	{
		if (M_I_T[j] < 0) 
			continue;

		if (Exclusion(j, M_I_T[j]))
			Problema(13);
	}
	for (size_t j = 0; j < M_Resources.size(); j++)
	{
		for (int i = 0; i < M_T; i++)
		{
			if (M_Resources[j].Get_Upper(i) < M_Resources[j].Get_Temp(i)- __FLT_EPSILON__)
			{
				printf("Valor Upper %.5f Valor Temp %.5f \n",M_Resources[j].Get_Upper(i),M_Resources[j].Get_Temp(i));
				Problema(11);
			}
			if (M_Resources[j].Get_Lower(i) > M_Resources[j].Get_Temp(i)+__FLT_EPSILON__)
				Problema(12);
		}
	}
	return false;

}
bool Maintenance::VerifySolution(list<pair<int,int>> sol)
{
	LimpiarVectores();
	for (list<pair<int, int>> ::iterator it = sol.begin(); it != sol.end(); it++)
	{
		M_Interventions_Colocada[(*it).first] = true;
		M_Interventions[(*it).first].Set_T_Temp((*it).second);
		if (Exclusion( (*it).first, (*it).second))
			Problema(13);
		for (vector<int>::iterator it2=M_Interventions[(*it).first].Get_Workload_Res().begin();it2!= M_Interventions[(*it).first].Get_Workload_Res().end();it2++)
		{
//		for (map<int, map<int, map <int, double>>> ::iterator it2 = M_Interventions[(*it).first].Get_Workload().begin(); it2 != M_Interventions[(*it).first].Get_Workload().end(); it2++)
//		{
			//Recursos
			for (int tprima = (*it).second; tprima < ((*it).second + M_Interventions[(*it).first].Get_Delta()[(*it).second]); tprima++)
			{

				M_Resources[(*it2)].Add(M_Interventions[(*it).first].Get_WorkloadN((*it2),(*it).second,tprima), tprima);
				//			if (M_Interventions[n_int].Get_Workload()[(*it).first][tprima][t] > 0 && (*it).first == 1 && tprima==6)
				//				cout << M_I_Name[n_int] << " " << tprima << " " << t << " " << M_R_Name[(*it).first] << " " << M_Interventions[n_int].Get_Workload()[(*it).first][tprima][t] << " Temp "<< M_Resources[(*it).first].Get_Temp()[tprima-1]<<  endl;

			}

		}
	}

	for (size_t j = 0; j < M_Resources.size(); j++)
	{
		for (int i = 0; i < M_T; i++)
		{
			if (M_Resources[j].Get_Upper()[i] < M_Resources[j].Get_Temp()[i] - __FLT_EPSILON__)
				Problema(11);
			if (M_Resources[j].Get_Lower()[i] > M_Resources[j].Get_Temp()[i] + __FLT_EPSILON__)
				Problema(12);
		}
	}
	return false;

}
bool Maintenance::VerifySolution2()
{
	LimpiarVectores();
	for (size_t i=0;i<M_N_Interventions;i++)
	{

//	for (list<pair<int, int>> ::iterator it = sol.begin(); it != sol.end(); it++)
//	{
		M_Interventions_Colocada[i] = true;
		M_Interventions[i].Set_T_Temp(M_I_T[i]);
		if (Exclusion(i, M_I_T[i]))
			Problema(13);
		for (vector<int>::iterator it2 = M_Interventions[i].Get_Workload_Res().begin(); it2 != M_Interventions[i].Get_Workload_Res().end(); it2++)
		{
			//Recursos
			for (int tprima = M_I_T[i]; tprima < (M_I_T[i] + M_Interventions[i].Get_Delta()[M_I_T[i]]); tprima++)
			{

				M_Resources[(*it2)].Add(M_Interventions[i].Get_WorkloadN((*it2), M_I_T[i],tprima), tprima);
				//			if (M_Interventions[n_int].Get_Workload()[(*it).first][tprima][t] > 0 && (*it).first == 1 && tprima==6)
				//				cout << M_I_Name[n_int] << " " << tprima << " " << t << " " << M_R_Name[(*it).first] << " " << M_Interventions[n_int].Get_Workload()[(*it).first][tprima][t] << " Temp "<< M_Resources[(*it).first].Get_Temp()[tprima-1]<<  endl;

			}

		}
	}

	for (size_t j = 0; j < M_Resources.size(); j++)
	{
		for (int i = 0; i < M_T; i++)
		{
			if (M_Resources[j].Get_Upper()[i] < M_Resources[j].Get_Temp()[i] - __FLT_EPSILON__)
				Problema(11);
			if (M_Resources[j].Get_Lower()[i] > M_Resources[j].Get_Temp()[i] + __FLT_EPSILON__)
				Problema(12);
		}
	}
	return false;

}
//Hay algunos t que no se pueden poner directamente por workload
bool Maintenance::PuedoPonerSola(Intervention& I, int t)
{
	for (vector<int>::iterator it =I.Get_Workload_Res().begin(); it != I.Get_Workload_Res().end(); it++)
	{

//	for (map<int, map<int, map <int, double>>> ::iterator it = I.Get_Workload().begin(); it != I.Get_Workload().end(); it++)
//	{
		//Recursos
		for (int tprima = t; tprima < (t + I.Get_Delta(t)); tprima++)
		{
			if ((I.Get_WorkloadN((*it),t,tprima)  > (M_Resources[(*it)].Get_Upper(tprima)+__FLT_EPSILON__)))
				return false;
		}
	}
	return true;
}
double Maintenance::FuncionObjetivoTemp()

{
	if (M_Pool == false)
	{
		M_fo2_temp = 0;
		return M_Alpha * (M_fo_temp / (double)M_T);

	}
	double fobjetivo_second = 0;
	double fobjetivo_first3 = 0;
//	vector<double> temp;
	for (int i = 0; i < M_T; i++)
	{

		int pos = 0;
//		temp.clear();
		if (M_Scenarios_number[i] > 1)
		{
			pos = ceil((double) M_Scenarios_number[i] * M_Quantile) - 1;
			for (int k = 0; k < M_Scenarios_number[i]; k++)
			{
				M_T_Temp_Risk_FO[i][k] = M_T_Temp_Risk_S[i][k];
//				temp.push_back(M_T_Temp_Risk_S[i][k]);
			}
			
		}
		else
		{
			M_T_Temp_Risk_FO[i][0] = M_T_Temp_Risk_S[i][0];
			continue;
		}
		double suma = accumulate(M_T_Temp_Risk_FO[i].begin(), M_T_Temp_Risk_FO[i].end(), 0.0);
		M_T_Temp_Risk_FO_Suma[i] = suma;
		double media = suma / M_Scenarios_number[i];
		nth_element(M_T_Temp_Risk_FO[i].begin(), M_T_Temp_Risk_FO[i].begin() + pos, M_T_Temp_Risk_FO[i].end());
		//		double cuantil1= M_T_Temp_Risk_S[i][pos];
		//		 sort(M_T_Temp_Risk_S[i].begin(), M_T_Temp_Risk_S[i].end());
		double cuantil = M_T_Temp_Risk_FO[i][pos];
		double excess = max((double) 0, (cuantil - media));
		fobjetivo_first3 += media;
		fobjetivo_second += excess;
		M_vfo2_temp[i] = excess;

	}
	M_fo2_temp = fobjetivo_second;
//	M_fo_temp = fobjetivo_first3;
	double total = M_Alpha * (M_fo_temp / (double)M_T) + (1 - M_Alpha) * (fobjetivo_second / (double)M_T);
	return total;
}
double Maintenance::FuncionObjetivoTemp(int numero_i,int ti1,int tf1)
{
	if (M_Pool == false)
	{
		M_fo2_temp = 0;
		return M_Alpha * (M_fo_temp / (double)M_T);

	}
	double fobjetivo_second = 0;

	//	vector<double> temp;
	for (int i = 0; i < M_T; i++)
	{
		//Si no es nuevo lo tengo calculado
		if (i >= ti1 && i < tf1)
		{
//			double tempkk = 0;
			if (M_vfo2_temp[i] < __FLT_EPSILON__ && M_T_Temp_Risk_S_Copy[i][M_Scenarios_number[i] - 1]< __FLT_EPSILON__)
			{


				M_vfo2_temp[i] = M_Interventions[numero_i].Get_Riskexcess(ti1, i);
				fobjetivo_second += M_vfo2_temp[i];
//				if (tempkk<excess - __FLT_EPSILON__ || tempkk>excess + __FLT_EPSILON__)
	//				Problema(127);
			}
			else
			{

				int pos = 0;

				//		temp.clear();
				double suma = 0;
//				double last_position = 0;
				if (M_Scenarios_number[i] > 1)
				{
	//				last_position = M_T_Temp_Risk_FO[i][M_Scenarios_number[i] - 1];
					pos = ceil((double)M_Scenarios_number[i] * M_Quantile) - 1;
					//				pos2 = M_Scenarios_number[i] - pos+1;
					for (int k = 0; k < M_Scenarios_number[i]; k++)
					{
						M_T_Temp_Risk_FO[i][k] = M_T_Temp_Risk_S[i][k];
						suma += M_T_Temp_Risk_FO[i][k];
						//				temp.push_back(M_T_Temp_Risk_S[i][k]);
					}

				}
				else
				{
					M_T_Temp_Risk_FO[i][0] = M_T_Temp_Risk_S[i][0];
					continue;
				}
				//			double suma = accumulate(M_T_Temp_Risk_FO[i].begin(), M_T_Temp_Risk_FO[i].end(), 0.0);
				M_T_Temp_Risk_FO_Suma[i]=suma;
				double media = suma / M_Scenarios_number[i];
				nth_element(M_T_Temp_Risk_FO[i].begin(), M_T_Temp_Risk_FO[i].begin() + pos, M_T_Temp_Risk_FO[i].end());
				double cuantil = M_T_Temp_Risk_FO[i][pos];
				double excess = max((double)0, (cuantil - media));
//				fobjetivo_first3 += media;
				fobjetivo_second += excess;
				M_vfo2_temp[i] = excess;
/*				double tempkk = 0;
				if (last_position < __FLT_EPSILON__)
				{

					tempkk = M_Interventions[numero_i].Get_Riskexcess(ti1, i);
					if (tempkk<excess - __FLT_EPSILON__ || tempkk>excess + __FLT_EPSILON__)
						Problema(127);
				}*/

			}
				

		}
		else
			fobjetivo_second += M_vfo2_temp[i];

	}
	M_fo2_temp = fobjetivo_second;

	double total = M_Alpha * (M_fo_temp / (double)M_T) + (1 - M_Alpha) * (fobjetivo_second / (double)M_T);
	return total;
}
double Maintenance::FuncionObjetivoTemp( int ti1, int tf1)
{
	if (M_Pool == false)
	{
		M_fo2_temp = 0;
		return M_Alpha * (M_fo_temp / (double)M_T);

	}
	double fobjetivo_second = 0;
	double fobjetivo_first3 = 0;
	//	vector<double> temp;
	for (int i = 0; i < M_T; i++)
	{
		//Si no es nuevo lo tengo calculado
		if (i >= ti1 && i < tf1)
		{

				int pos = 0;

				//		temp.clear();
				double suma = 0;
				if (M_Scenarios_number[i] > 1)
				{
					pos = ceil((double)M_Scenarios_number[i] * M_Quantile) - 1;
					//				pos2 = M_Scenarios_number[i] - pos+1;
					for (int k = 0; k < M_Scenarios_number[i]; k++)
					{
						M_T_Temp_Risk_FO[i][k] = M_T_Temp_Risk_S[i][k];
						suma += M_T_Temp_Risk_FO[i][k];
						//				temp.push_back(M_T_Temp_Risk_S[i][k]);
					}

				}
				else
				{
					M_T_Temp_Risk_FO[i][0] = M_T_Temp_Risk_S[i][0];
					continue;
				}
				M_T_Temp_Risk_FO_Suma[i] = suma;
				//			double suma = accumulate(M_T_Temp_Risk_FO[i].begin(), M_T_Temp_Risk_FO[i].end(), 0.0);
				double media = suma / M_Scenarios_number[i];
				nth_element(M_T_Temp_Risk_FO[i].begin(), M_T_Temp_Risk_FO[i].begin() + pos, M_T_Temp_Risk_FO[i].end());
				double cuantil = M_T_Temp_Risk_FO[i][pos];
				double excess = max((double)0, (cuantil - media));
				fobjetivo_first3 += media;
				fobjetivo_second += excess;
				M_vfo2_temp[i] = excess;

		}
		else
			fobjetivo_second += M_vfo2_temp[i];

	}
	M_fo2_temp = fobjetivo_second;

	double total = M_Alpha * (M_fo_temp / (double)M_T) + (1 - M_Alpha) * (fobjetivo_second / (double)M_T);
	return total;
}

double Maintenance::FuncionObjetivoPenalizacion()
{


	vector<int> NumIT(M_T);
  // Primero da un warning de que interventions no están colocadas
	for (size_t i=0; i < M_Interventions.size(); i++)
	{
		if (M_I_T[i] <0)
			cout << "La intervention " << i << "no está colocada"<<endl;
	}
	//Parte del riesgo acumulado
	//
	LimpiarRiskTempS2();
	//



	double fobjetivo_first2 = 0;

	for (size_t i=0; i < M_Interventions.size(); i++)
	{
		
		if (M_I_T[i] < 0) continue;
		double acumuladoT=0;
		for (int t = M_Interventions[i].Get_T_Temp(); t < M_Interventions[i].Get_T_F_Temp(); t++)
		{
			M_FreqIT[i][t]++;
			NumIT[t]++;
			double acumuladoT2=0;
			for (int si = 0; si < M_Scenarios_number[t]; si++)
			{

				acumuladoT2 += M_Interventions[i].Get_RiskN(t, M_Interventions[i].Get_T_Temp(), si);
//				InsertarOrdenadoTRisk(t, M_Interventions[i].Get_Risk()[t][M_Interventions[i].Get_T_Temp()][si]);

				M_T_Temp_Risk_S2[t][si] += M_Interventions[i].Get_RiskN(t, M_Interventions[i].Get_T_Temp(),si);

//				kktot += M_Interventions[i].Get_Risk()[t][M_Interventions[i].Get_T_Temp()][si];
			}
			acumuladoT2 = acumuladoT2/ M_Scenarios_number[t];
			acumuladoT += acumuladoT2;
		}
		
		fobjetivo_first2 += acumuladoT;
	}
	M_NumIT = NumIT;
	double fobje2 = 0;
	for (size_t i=0; i < M_Interventions.size(); i++)
	{
		if (M_I_T[i] < 0) continue;


		fobje2 += M_Interventions[i].Get_RiskC(M_Interventions[i].Get_T_Temp());
		double dif = M_Interventions[i].Get_RiskC(M_Interventions[i].Get_T_Temp()) - M_Interventions[i].Get_Min_Risk();

	}
	double fobjetivo_first3 = 0;
	double fobjetivo_second = 0;

	for (int i = 0; i < M_T; i++)
	{

		int pos = 0;
		double dif = (double)(1 - M_Quantile) / (double)2;
		if (M_Scenarios_number[i] > 0)
		{
			pos = ceil(M_Scenarios_number[i] * M_Quantile) - 1;
		}
		else continue;
		double suma=accumulate(M_T_Temp_Risk_S2[i].begin(), M_T_Temp_Risk_S2[i].end(), 0.0);
		M_T_Temp_Risk_FO_Suma[i] = suma;
		double media = suma / M_Scenarios_number[i];
		auto risk_scenario = M_T_Temp_Risk_S2[i];
		nth_element(M_T_Temp_Risk_S2[i].begin(), M_T_Temp_Risk_S2[i].begin() + pos, M_T_Temp_Risk_S2[i].end());
//		double cuantil1= M_T_Temp_Risk_S[i][pos];
//		 sort(M_T_Temp_Risk_S[i].begin(), M_T_Temp_Risk_S[i].end());
		 double cuantil = M_T_Temp_Risk_S2[i][pos];
		 double excess = max((double) 0, (cuantil - media));
		 int index = ceil(M_Quantile * M_Scenarios_number[i]) - 1;
		 int index_arriba = ceil((M_Quantile+dif) * M_Scenarios_number[i]) - 1;
		 nth_element(M_T_Temp_Risk_S2[i].begin(), M_T_Temp_Risk_S2[i].begin() + index_arriba,
		             M_T_Temp_Risk_S2[i].end());
		 double cuantil_arriba = M_T_Temp_Risk_S2[i][index_arriba];
		 int index_abajo = ceil((M_Quantile - dif) * M_Scenarios_number[i]) - 1;
		 nth_element(M_T_Temp_Risk_S2[i].begin(),
		             M_T_Temp_Risk_S2[i].begin() + index_abajo, M_T_Temp_Risk_S2[i].end());
		 double cuantil_abajo= M_T_Temp_Risk_S2[i][index_abajo];
		 fobjetivo_first3 += media;
		 fobjetivo_second += excess;
		 M_Excess[i] = excess ;
		 //Penalizacion
		 double umbral = 0.2;
		 int scenarios_penalizo = 0;
		 /* for (auto s = 0; s < M_Scenarios_number[i]; s++)
		 {
			 M_Scenario_Activo[i][s] = true;
			 if ((risk_scenario[s] > cuantil - (umbral * excess)) &&
			     (risk_scenario[s] < cuantil + (umbral * excess)))
				 scenarios_penalizo++;
		 }*/
		
		 
		 if (excess > 0) 
		 {
			 double vp = (excess) / (M_Scenarios_number[i] - index);
//			 vp = (excess) / (scenarios_penalizo);
		//	 double vp2 = (excess) /(M_Scenarios_number[i]- (M_Scenarios_number[i] - index));

			 vp = vp * M_Scenarios_number[i];
		//	 vp2 = vp2 * M_Scenarios_number[i];
			 for (auto s = 0; s < M_Scenarios_number[i]; s++)
			 {

//				 if (risk_scenario[s] > cuantil )
//				 {
					 
//					 if (risk_scenario[s] > (cuantil + 0.25*excess))
//						 continue;
// 
						 //					 if (M_Freq[i][s] == 0)
//					 {
if (risk_scenario[s] > cuantil_abajo && risk_scenario[s] < cuantil_arriba)
					{
	//				if ((risk_scenario[s] > cuantil - (umbral * excess)) &&
//				(risk_scenario[s] < cuantil + (umbral * excess)))
//{
	if (M_Tipo_Coeficientes != 3)
		M_PenalScen[i][s] = M_PenalScen[i][s] + vp;
	else
		M_PenalScen[i][s] = max(M_PenalScen[i][s], vp);
						 M_Freq[i][s]++;
//					 }
					 M_Scenario_Activo[i][s] = false;
				 }
				 /*
				 if (risk_scenario[s] <= cuantil)
				 {
					 M_PenalScen[i][s] = M_PenalScen[i][s] + vp2;
					 M_Freq[i][s]++;
					 M_Scenario_Activo[i][s] = false;
				 }	*/			 

			 }

		 }




//		 if (M_vfo2_temp[i]<excess - 0.01 || M_vfo2_temp[i]>excess + 0.01)
//			 Problema(122);
	}

	for (size_t i = 0; i < M_Interventions.size(); i++)
	{

		if (M_I_T[i] < 0)
			continue;
		for (int t = M_Interventions[i].Get_T_Temp(); t < M_Interventions[i].Get_T_F_Temp();
		     t++)
		{
			M_PenalScenIT[i][t] += (M_Excess[t]/NumIT[t]);
		}
	}
	double total = M_Alpha * (fobjetivo_first3 / (double)M_T) + (1 - M_Alpha) * (fobjetivo_second / (double)M_T);
	if (!M_Challenge_Mode)
		printf("Primera Parte3 %.3f Segunda Parte %.3f Total %.3f\n", fobjetivo_first3 / (double)M_T * 0.5, fobjetivo_second / (double)M_T * 0.5, total);
//		cout << "Primera Parte3 " << fobjetivo_first3 / (double)M_T << " segunda parte " << fobjetivo_second/(double)M_T << " Total "<< total << " Tipo " << M_Tipo<<endl;
//	cout << "Primera Parte3 " << fobjetivo_first3 / (double)M_T << " segunda parte " << fobjetivo_second / (double)M_T << " Total " << total << " Tipo " << M_Tipo << endl;


/*	for (size_t i=0; i < M_Interventions.size(); i++)
	{
		if (M_Interventions_Colocada[i] == false) continue;
		fobjetivo_first += M_Interventions[i].Get_RiskC()[M_Interventions[i].Get_T_Temp()];
	}
	cout << "Segunda Parte " << fobjetivo_first / (double)M_T;
	*/
//	M_fo_temp = total;
	return total;

}
double Maintenance::FuncionObjetivo()
{

	// Primero da un warning de que interventions no están colocadas
	for (size_t i = 0; i < M_Interventions.size(); i++)
	{
		if (M_I_T[i] < 0)
			cout << "La intervention " << i << "no está colocada" << endl;
	}
	//Parte del riesgo acumulado
	//
	LimpiarRiskTempS2();
	//

	double fobjetivo_first2 = 0;

	for (size_t i = 0; i < M_Interventions.size(); i++)
	{
		if (M_I_T[i] < 0)
			continue;
		double acumuladoT = 0;
		for (int t = M_Interventions[i].Get_T_Temp(); t < M_Interventions[i].Get_T_F_Temp();
		     t++)
		{
			double acumuladoT2 = 0;
			for (int si = 0; si < M_Scenarios_number[t]; si++)
			{
				acumuladoT2 += M_Interventions[i].Get_RiskN(
				    t, M_Interventions[i].Get_T_Temp(), si);
				//				InsertarOrdenadoTRisk(t, M_Interventions[i].Get_Risk()[t][M_Interventions[i].Get_T_Temp()][si]);

				M_T_Temp_Risk_S2[t][si] += M_Interventions[i].Get_RiskN(
				    t, M_Interventions[i].Get_T_Temp(), si);

				//				kktot += M_Interventions[i].Get_Risk()[t][M_Interventions[i].Get_T_Temp()][si];
			}
			acumuladoT2 = acumuladoT2 / M_Scenarios_number[t];
			acumuladoT += acumuladoT2;
		}

		fobjetivo_first2 += acumuladoT;
	}
	double fobje2 = 0;
	for (size_t i = 0; i < M_Interventions.size(); i++)
	{
		if (M_I_T[i] < 0)
			continue;

		fobje2 += M_Interventions[i].Get_RiskC(M_Interventions[i].Get_T_Temp());
		double dif = M_Interventions[i].Get_RiskC(M_Interventions[i].Get_T_Temp()) -
		             M_Interventions[i].Get_Min_Risk();
	}
	double fobjetivo_first3 = 0;
	double fobjetivo_second = 0;

	for (int i = 0; i < M_T; i++)
	{

		int pos = 0;
		if (M_Scenarios_number[i] > 0)
		{
			pos = ceil(M_Scenarios_number[i] * M_Quantile) - 1;
		}
		else
			continue;
		double suma =
		    accumulate(M_T_Temp_Risk_S2[i].begin(), M_T_Temp_Risk_S2[i].end(), 0.0);
		M_T_Temp_Risk_FO_Suma[i] = suma;
		double media = suma / M_Scenarios_number[i];
		nth_element(M_T_Temp_Risk_S2[i].begin(), M_T_Temp_Risk_S2[i].begin() + pos,
		            M_T_Temp_Risk_S2[i].end());
		//		double cuantil1= M_T_Temp_Risk_S[i][pos];
		//		 sort(M_T_Temp_Risk_S[i].begin(), M_T_Temp_Risk_S[i].end());
		double cuantil = M_T_Temp_Risk_S2[i][pos];
		double excess = max((double)0, (cuantil - media));
		fobjetivo_first3 += media;
		fobjetivo_second += excess;
		//		 if (M_vfo2_temp[i]<excess - 0.01 || M_vfo2_temp[i]>excess + 0.01)
		//			 Problema(122);
	}
	double total = M_Alpha * (fobjetivo_first3 / (double)M_T) +
	               (1 - M_Alpha) * (fobjetivo_second / (double)M_T);
	if (!M_Silent)
		cout << "Primera Parte3 " << fobjetivo_first3 / (double)M_T << " segunda parte "
		     << fobjetivo_second / (double)M_T << " Total " << total << " Tipo " << M_Tipo
		     << endl;
	//	cout << "Primera Parte3 " << fobjetivo_first3 / (double)M_T << " segunda parte " << fobjetivo_second / (double)M_T << " Total " << total << " Tipo " << M_Tipo << endl;

	/*	for (size_t i=0; i < M_Interventions.size(); i++)
	{
		if (M_Interventions_Colocada[i] == false) continue;
		fobjetivo_first += M_Interventions[i].Get_RiskC()[M_Interventions[i].Get_T_Temp()];
	}
	cout << "Segunda Parte " << fobjetivo_first / (double)M_T;
	*/
	//	M_fo_temp = total;
	return total;
}


double Maintenance::FuncionObjetivo(vector<int> Interventions_T)
{
	// Primero da un warning de que interventions no están colocadas
	for (int j = 0; j < M_N_Interventions; j++)
	{
		if (Interventions_T[j] < 0)
		{
			cout << "La intervention " << j << "no está colocada" << endl;
			return 0;
		}
	}

	LimpiarRiskTempS();

	//Parte del riesgo acumulado
	double fobjetivo_first2 = 0;

	for (size_t i=0; i < M_Interventions.size(); i++)
	{
		double acumuladoT = 0;
		for (int t = Interventions_T[i]; t < Interventions_T[i]+M_Interventions[i].Get_Delta()[M_Interventions_T[i]]; t++)
		{
			double acumuladoT2 = 0;
			for (int si = 0; si < M_Scenarios_number[t]; si++)
			{
				acumuladoT2 += M_Interventions[i].Get_Risk()[t][Interventions_T[i]][si];
				//				InsertarOrdenadoTRisk(t, M_Interventions[i].Get_Risk()[t][M_Interventions[i].Get_T_Temp()][si]);

				M_T_Temp_Risk_S[t][si] += M_Interventions[i].Get_Risk()[t][Interventions_T[i]][si];

//				kktot += M_Interventions[i].Get_Risk()[t][Interventions_T[i]][si];
			}
			acumuladoT2 = acumuladoT2 / M_Scenarios_number[t];
			acumuladoT += acumuladoT2;
		}

		fobjetivo_first2 += acumuladoT;
	}

	double fobjetivo_first3 = 0;
	double fobjetivo_second = 0;

	for (int i = 0; i < M_T; i++)
	{

		int pos = 0;
		if (M_T_Temp_Risk_S[i].size() > 0)
		{
			pos = ceil(M_Scenarios_number[i] * M_Quantile) - 1;
		}
		else continue;
		double suma = accumulate(M_T_Temp_Risk_S[i].begin(), M_T_Temp_Risk_S[i].end(), 0.0);
		double media = suma / M_Scenarios_number[i];
		nth_element(M_T_Temp_Risk_S[i].begin(), M_T_Temp_Risk_S[i].begin() + pos, M_T_Temp_Risk_S[i].end());
		//		double cuantil1= M_T_Temp_Risk_S[i][pos];
		//		 sort(M_T_Temp_Risk_S[i].begin(), M_T_Temp_Risk_S[i].end());
		double cuantil = M_T_Temp_Risk_S[i][pos];
		double excess = max((double) 0, (cuantil - media));
		fobjetivo_first3 += media;
		fobjetivo_second += excess;


	}
	double total = M_Alpha * (fobjetivo_first3 / (double)M_T) + (1 - M_Alpha) * (fobjetivo_second / (double)M_T);
	if (!M_Silent) cout << "Primera Parte3 " << fobjetivo_first3 / (double)M_T << " segunda parte " << fobjetivo_second / (double)M_T << " Total " << total << " Tipo " << M_Tipo << endl;



	return total;

}
void Maintenance::InsertarGoodSolutions(list<pair<vector<int>, double>> &lista)
{

	for (list<pair<vector<int>, double>> ::iterator it = lista.begin(); it != lista.end(); it++)
	{

//		M_I_T = (*it).first;
		PoolGoodSolutions( (*it).first,(*it).second);
	}
}
void Maintenance::InsertarGoodSolutions_Diver(list<pair<vector<int>, double>> &lista)
{

	for (list<pair<vector<int>, double>> ::iterator it2 = lista.begin(); it2 != lista.end(); it2++)
	{


		bool distinta = true;
		int distan = 0;
		for (list<pair<vector<int>, double>> ::iterator it = M_PoolGoodSolutions_diver.begin(); distinta!=false && it != M_PoolGoodSolutions_diver.end(); it++)
		{
			distan = Distancia((*it2).first, (*it).first);
			if (distan < M_MinDistintosPool) 
				distinta = false;
		}
	for (list<pair<vector<int>, double>> ::iterator it = M_PoolGoodSolutions.begin(); distinta!=false && it != M_PoolGoodSolutions.end(); it++)
		{
			distan = Distancia((*it2).first, (*it).first);
			if (distan < M_MinDistintosPool) 
				distinta = false;
		}
		if (distinta == false)
		{

			return;
		}

		for (list<pair<vector<int>, double>> ::iterator it = M_PoolGoodSolutions_diver.begin(); it != M_PoolGoodSolutions_diver.end(); it++)
		{
			if (((*it2).second) < ((*it).second - __FLT_EPSILON__))
			{

				pair<vector<int>, double> p((*it).first,(*it2).second);
				M_PoolGoodSolutions_diver.insert(it, p);
				if (M_PoolGoodSolutions_diver.size() > M_SizePoolSolutions)
				{

					M_PoolGoodSolutions_diver.pop_back();
					M_MinPoolSolutions_diver = M_PoolGoodSolutions_diver.back().second;
				}

				return;
			}
		}

		if ( M_PoolGoodSolutions_diver.size() < M_SizePoolSolutions)
		{

			pair<vector<int>, double> p((*it2).first,(*it2).second);
			M_PoolGoodSolutions_diver.push_back(p);
			return;
		}

	}
}
void Maintenance::InsertarOrdenadoPool(double val, vector<int> sol, list<pair<vector<int>, double>>& lista)
{
//	PoolGoodSolutions(sol, val);
	//Quito 22062021
/*	int ndistintos = M_distintos.size();
	M_distintos.insert(hash_operator(M_I_T));
	if (ndistintos == M_distintos.size())
		return;*/

	ActualizarMejorSolution(val, 1001);

	for (list<pair<vector<int>,double>> ::iterator it =lista.begin(); it != lista.end(); it++)
	{
		if ((*it).second < val - __FLT_EPSILON__)
		{
			pair<vector<int>, double> p(sol, val);
			lista.insert(it, p);
			return;
		}
	}
	pair<vector<int>, double> p(sol, val);
	lista.push_back(p);
}
void Maintenance::InsertarOrdenadoTRisk(int tiempo, double val, bool creciente)
{
	if (creciente == false)
	{
		for (list<double> ::iterator it=M_T_Temp_Risk[tiempo].begin();it!=M_T_Temp_Risk[tiempo].end();it++)
		{
			if ((*it) > val + __FLT_EPSILON__)
			{
				M_T_Temp_Risk[tiempo].insert(it, val);
				return;
			}
		}
		M_T_Temp_Risk[tiempo].push_back( val);
	}
	else
	{
		for (list<double> ::iterator it = M_T_Temp_Risk[tiempo].begin(); it != M_T_Temp_Risk[tiempo].end(); it++)
		{
			if ((*it) < val - __FLT_EPSILON__)
			{
				M_T_Temp_Risk[tiempo].insert(it, val);
				return;
			}
		}
		M_T_Temp_Risk[tiempo].push_back(val);
	}
}

int Maintenance::Min(vector<double>& a)
{
	int elegido_min = -1;
	double min = INT_LEAST32_MAX;
	for (size_t i=0; i < a.size(); i++)
	{
		if (a[i] > __FLT_EPSILON__ && a[i] < min)
		{
			min = a[i];
			elegido_min = i;
		}
	}
	if (elegido_min == -1) Problema(27);

	return elegido_min;

}
int Maintenance::Max(vector<double>& a)
{
	int elegido_max = -1;
	double max = __FLT_EPSILON__;
	for (size_t i=0; i < a.size(); i++)
	{
		if (a[i] > max)
		{
			max = a[i];
			elegido_max = i;
		}
	}
	if (elegido_max == -1) Problema(28);

	return elegido_max;
}
void Maintenance::Problema(int a)
{
	for (size_t i=0;i<10000; i++)
		cout << "Problema " << a;
}
int Maintenance::Pos(int val, vector<pair<int, double>>& Vector)
{
	for (size_t i=0;i<Vector.size(); i++)
	{
		if (Vector[i].first == val)
			return i;
	}
	Problema(23);
	return 0;
}
void Maintenance::VectoresOrdenados(void)
{

	//Para la función objetivo

	M_T_Temp_Risk.clear();
	M_T_Temp_Risk_S.clear();
	vector<double> suma(M_T);
	M_T_M1_Suma = suma;
	M_T_M2_Suma = suma;
	M_T_M3_Suma = suma;
	M_T_M4_Suma = suma;
	M_BestT_Suma = suma;
	vector<bool> b(M_T);
	M_T_M1 = b;
	M_T_M2 = b;
	M_T_M3 = b;
	M_T_M4 = b;
	for (int i = 0; i < M_T; i++)
	{
		M_T_M1[i] = true;
		M_T_M2[i] = true;
		M_T_M3[i] = true;
		M_T_M4[i] = true;
	}
	vector<vector<double>> p1(M_T);
	vector<vector<int>> pint(M_T);
	M_T_Temp_Risk_S = p1;
	M_T_Temp_Risk_S2 = p1;
	M_Coef = p1;
	M_PenalScen = p1;
	M_Best_Coef = p1;
	M_Best_Freq = p1;
	M_T_Temp_Risk_FO = p1;
	vector<vector<bool>> pbool(M_T);
	M_Scenario_Activo = pbool;
	vector<double> kk(M_T);
	
	M_T_Temp_Risk_FO_Suma = kk;
	for (int i = 0; i < M_N_Interventions; i++) 
	{
		M_FreqIT.push_back(vector<int> (M_T));
		M_PenalScenIT.push_back(vector<double> (M_T));
		M_CoefIT.push_back(vector<double>(M_T));
	}
	for (int i = 0; i < M_T; i++)
	{
		M_NumIT.push_back(0);
		M_Excess.push_back(0);
		M_ExcessXI.push_back(0);
		M_Best_Quantile.push_back(0);
		M_Best_Media.push_back(0);
		M_T_Excess.push_back(i);
		M_vfo2_temp.push_back(0);
		M_vfo2_temp_Copy.push_back(0);
		M_vfo2_temp_Copy2.push_back(0);
		M_vfo2_temp_Copy3.push_back(0);
		M_Ivfo2_temp.push_back(i);
		M_Best_Quantile.push_back(0);
		M_Best_Media.push_back(0);

		if (M_T_Temp_Risk_S[i].size() == 0)
		{
			vector<double> p(M_Scenarios_number[i]);
			vector<int> p_int(M_Scenarios_number[i]);
			vector<bool> p_bool(M_Scenarios_number[i]);
			M_T_Temp_Risk_S[i] = p;
			M_T_Temp_Risk_S2[i] = p;

			M_T_Temp_Risk_FO[i] = p;
			M_Scenario_Activo[i] = p_bool;
			M_Coef[i] = p;
			M_Freq.push_back(p_int);
			M_PenalScen[i] = p;
			M_Best_Coef[i] = p;
			M_Best_Freq[i] = p;
		}
		for (int t = 0; t < M_Scenarios_number[i]; t++) 
		{
			M_Scenario_Activo[i][t]=(true);
			M_Coef[i][t] = 1;
			M_Freq[i][t] = 0;

		}
	}

	M_Interventions_Regret.clear();
	M_Interventions_MinWxT.clear();
	M_Interventions_MinRxT.clear();
	M_Interventions_Min_WXR.clear();
	M_Interventions_Min_WtXRt.clear();
	M_Interventions_MinW.clear();
	M_Interventions_MinR.clear();
	M_Interventions_MaxDifW.clear();
	M_Interventions_MaxDifR.clear();
	M_Interventions_MinWxT_I.clear();
	M_Interventions_MinRxT_I.clear();
	M_Interventions_MinW_I.clear();
	M_Interventions_MinR_I.clear();
	M_Interventions_MaxDifW_I.clear();
	M_Interventions_MaxDifR_I.clear();
	M_Interventions_Exclusions_I.clear();
	M_Interventions_Temp.clear();
	M_Interventions_T.clear();
	for (int j = 0; j < M_T; j++)
	{
		M_Vec_Alea_T.push_back(j);
	}
	for (int j = 0; j < M_N_Interventions; j++)
	{
		M_I_Seasons.push_back(-1);
		M_Interventions_Colocada.push_back(-1);
		//		M_Best_I_T.push_back(-1);
		M_Interventions_Dif.push_back(j);
		M_I_T_Copy3.push_back(-1);
		M_I_T_Copy2.push_back(-1);
		M_I_T_Copy.push_back(-1);
		M_I_T.push_back(-1);
		//		M_I_T_For.push_back(-1);
		//		M_I_T_ForQ.push_back(-1);
		M_Vec_Alea_I.push_back(j);
		M_Interventions_T.push_back(-1);
		M_Interventions_T_Copy.push_back(-1);
		M_Interventions_T_Copy2.push_back(-1);
		M_Interventions_T_Copy3.push_back(-1);
		M_Interventions_Colocada_Copy.push_back(false);
		M_I_Seasons_Copy.push_back(-1);
		M_Interventions_Colocada_Copy2.push_back(false);
		M_I_Seasons_Copy2.push_back(-1);
		M_Interventions_Colocada_Copy3.push_back(false);
		M_I_Seasons_Copy3.push_back(-1);
		M_Interventions_Out.push_back(0);
		M_Interventions_Out_Ordenada.push_back(j);
		M_Interventions_Regret.push_back(j);
		M_Interventions_Risk_Temp.push_back(j);
		//InsertarOrdenado(M_Interventions_Regret, j, M_Interventions[j].Get_Regret(), false);
		double temp = (double)M_Interventions[j].Get_Exclusions().size() * (double)M_T +
		              (double)M_Interventions[j].Get_Min_Delta() +
		              M_Interventions[j].Get_Min_Workload();
		InsertarOrdenado(M_Interventions_Exclusions, j, temp, false);
		InsertarOrdenado(M_Interventions_MinW, j, M_Interventions[j].Get_Min_Workload(),
		                 false);
		InsertarOrdenado(M_Interventions_MinR, j, M_Interventions[j].Get_Min_Risk(), false);
		InsertarOrdenado(M_Interventions_MinWxT, j, M_Interventions[j].Get_Min_WorkloadxT(),
		                 false);
		InsertarOrdenado(M_Interventions_MinRxT, j, M_Interventions[j].Get_Min_RiskxT(),
		                 false);
		InsertarOrdenado(M_Interventions_MaxDifW, j,
		                 M_Interventions[j].Get_Max_Workload() -
		                     M_Interventions[j].Get_Min_Workload(),
		                 false);
		InsertarOrdenado(
		    M_Interventions_MaxDifR, j,
		    M_Interventions[j].Get_Max_Risk() - M_Interventions[j].Get_Min_Risk(), false);

		InsertarOrdenado(M_Interventions_Min_WXR, j,
		                 2 * M_N_Interventions - Pos(j, M_Interventions_MinR) -
		                     Pos(j, M_Interventions_MinW),
		                 false);
		InsertarOrdenado(M_Interventions_Min_WtXRt, j,
		                 2 * M_N_Interventions - Pos(j, M_Interventions_MinRxT) -
		                     Pos(j, M_Interventions_MinWxT),
		                 false);
	}
	sort(M_Interventions_Regret.begin(), M_Interventions_Regret.end(), [&](size_t i, size_t j) {
		return M_Interventions[i].Get_Regret() > M_Interventions[j].Get_Regret();
	});
	sort(M_Interventions_Risk_Temp.begin(), M_Interventions_Risk_Temp.end(),
	     [&](size_t i, size_t j) {
		     return M_Interventions[i].Get_Min_Risk_Temp() >
		            M_Interventions[j].Get_Min_Risk_Temp();
	     });

	double val1 = __DBL_MAX__;
	double val2 = __DBL_MAX__;
	double val3 = __DBL_MAX__;
	double val4 = __DBL_MAX__;
	double val5 = __DBL_MAX__;
	double val6 = __DBL_MAX__;
	double val7 = __DBL_MAX__;
	double val8 = __DBL_MAX__;
	double val9 = __DBL_MAX__;

	for (int i = 1; i < M_N_Interventions; i++)
	{
		M_Interventions[i].Limpiar();
		if (M_Interventions_MinW[i].second < (val1 - __FLT_EPSILON__))
		{
			val1 = M_Interventions_MinW[i].second;
			M_Interventions_MinW_I.push_back(i);
		}
		if (M_Interventions_MinR[i].second < (val2 - __FLT_EPSILON__))
		{
			val2 = M_Interventions_MinR[i].second;
			M_Interventions_MinR_I.push_back(i);
		}
		if (M_Interventions_MinWxT[i].second < (val3 - __FLT_EPSILON__))
		{
			val3 = M_Interventions_MinWxT[i].second;
			M_Interventions_MinWxT_I.push_back(i);
		}
		if (M_Interventions_MinRxT[i].second < (val4 - __FLT_EPSILON__))
		{
			val4 = M_Interventions_MinRxT[i].second;
			M_Interventions_MinRxT_I.push_back(i);
		}
		if (M_Interventions_MaxDifW[i].second < (val5 - __FLT_EPSILON__))
		{
			val5 = M_Interventions_MaxDifW[i].second;
			M_Interventions_MaxDifW_I.push_back(i);
		}
		if (M_Interventions_MaxDifR[i].second < (val6 - __FLT_EPSILON__))
		{
			val6 = M_Interventions_MaxDifR[i].second;
			M_Interventions_MaxDifR_I.push_back(i);
		}
		if (M_Interventions_Min_WXR[i].second < (val7 - __FLT_EPSILON__))
		{
			val7 = M_Interventions_Min_WXR[i].second;
			M_Interventions_Min_WXR_I.push_back(i);
		}
		if (M_Interventions_Min_WtXRt[i].second < (val8 - __FLT_EPSILON__))
		{
			val8 = M_Interventions_Min_WtXRt[i].second;
			M_Interventions_Min_WtXRt_I.push_back(i);
		}
		if (M_Interventions_Exclusions[i].second < (val9 - __FLT_EPSILON__))
		{
			val9 = M_Interventions_Exclusions[i].second;
			M_Interventions_Exclusions_I.push_back(i);
		}
	}
	M_Interventions_MinWxT_I.push_back(M_Interventions_MinWxT.size());
	M_Interventions_MinRxT_I.push_back(M_Interventions_MinRxT.size());
	M_Interventions_MinW_I.push_back(M_Interventions_MinW.size());
	M_Interventions_MinR_I.push_back(M_Interventions_MinR.size());
	M_Interventions_MaxDifW_I.push_back(M_Interventions_MaxDifW.size());
	M_Interventions_MaxDifR_I.push_back(M_Interventions_MaxDifR.size());
	M_Interventions_Exclusions_I.push_back(M_Interventions_Exclusions.size());
	M_Interventions_Min_WXR_I.push_back(M_Interventions_Min_WXR.size());
	M_Interventions_Min_WtXRt_I.push_back(M_Interventions_Min_WtXRt.size());
	M_Max_Min_Riesgo = 0;
	M_Max_Riesgo = 0;
	for (int j = 0; j < M_T; j++)
	{
		for (size_t i = 0; i < M_N_Interventions; i++)
		{
			if (M_Max_Riesgo < M_Interventions[i].Get_RiskC(j))
				M_Max_Riesgo = M_Interventions[i].Get_RiskC(j);
		}
	}
	for (size_t i = 0; i < M_N_Interventions; i++)
	{
		if (M_Max_Min_Riesgo < M_Interventions_MinR[i].second)
			M_Max_Min_Riesgo = M_Interventions_MinR[i].second;
	}
	M_Max_Riesgo += 0.1;
	//Menor valor primera parte f_obj
	M_Min_fo = 0;
	for (size_t i = 0; i < M_N_Interventions; i++) 
	{
		M_Min_fo+=M_Interventions[i].Get_Min_Risk();
	}
	M_Min_fo_temp = M_Min_fo;
}
void Maintenance::InsertarOrdenado(vector<pair<int,double >> &Vec, int pos, double val, bool decreciente)
{
	if (decreciente == true)
	{
		for (size_t i=0; i < Vec.size(); i++)
		{
			if (Vec[i].second > val + __FLT_EPSILON__)
			{
				Vec.insert(Vec.begin() + i, pair<int, double>(pos, val));
				return;
			}
		}
		Vec.push_back(pair<int, double>(pos, val));
	}
	else
	{
		for (size_t i=0; i < Vec.size(); i++)
		{
			if (Vec[i].second < val - __FLT_EPSILON__)
			{
				Vec.insert(Vec.begin() + i, pair<int, double>(pos, val));
				return;
			}
		}
		Vec.push_back(pair<int, double>(pos, val));

	}

}
void Maintenance::PrintSolution(string &name)
{
	if (M_Best_I_T.size() == 0)
	{
		printf("NO HAY SOLUCION %%%%%%%%%%%%%%%%%%%%%%%%%%%%");
		printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");
		return;
	}
//	ofstream file(name, ios_base::binary | ios_base::out);
	ofstream file;
	file.open(name);
	for (int i=0;i<M_N_Interventions;i++)//list<pair<int, int>>::iterator it = M_Best_I_T_Solution.begin(); it != M_Best_I_T_Solution.end(); it++)
	{
		file << M_Interventions[i].Get_Name() << " " << M_Best_I_T[i] + 1 << endl;
	}
/*	for (list<pair<int,int>>::iterator it=M_Best_I_T_Solution.begin();it!=M_Best_I_T_Solution.end();it++)
	{
		file << M_Interventions[(*it).first].Get_Name() << " "  << (*it).second+1 << endl;
	}*/
//	file << endl;
	file.close();

}
void Maintenance::PrintSolutionStatistics(string& name, vector<int> &lista)
{
	//	ofstream file(name, ios_base::binary | ios_base::out);
	LimpiarVectores();
	ofstream file;
	file.open(name);
	for (int i=0;i<M_N_Interventions;i++)//list<pair<int, int>>::iterator it = lista.begin(); it != lista.end(); it++)
	{
//		ColocarEnTiempo((*it).first, (*it).second);
		file << M_Interventions[i].Get_Name() << " " <<lista[i] + 1 << endl;
	}
	file << endl;


	for (int i = 0; i < M_N_Interventions; i++)//list<pair<int, int>>::iterator it = lista.begin(); it != lista.end(); it++)
	{
		ColocarIEnTiempo(i, lista[i]);
		ModificarRiskTempAll(M_Interventions[i].Get_T_Temp(), M_Interventions[i].Get_T_F_Temp());

//		file << M_Interventions[(*it).first].Get_Name() << " " << (*it).second + 1 << endl;
	}

	for (size_t i=0; i < M_N_Interventions; i++)
	{
		file << M_Interventions_T[i] << " ";
	}
	file << endl;
	PrintValores();
	//	file << endl;
	file.close();

}

void Maintenance::leer_problem_wojsonOLD(string& name_file)
{
	// open the json file - here replaced with a istringstream containing the json data


	struct timeb t1;
	M_Name_file = name_file;
	string name_file2 = name_file + ".json";
	ifstream ifs(name_file2);
	if (!ifs.is_open()) {

		cout << "El fichero " << name_file2 << "está abierto" << endl;
		return;
	}
	ftime(&t1);
	M_Time_Initial = t1;

	//Primero leemos valores

	string str;
	while (getline(ifs, str))
	{
		size_t index = 0;
		if (str.find("\"T\"", index) != str.npos)
		{
			size_t kk = str.find(":");
			size_t pos = str.find(",");
			string kk3 = str.substr(kk + 1, pos - kk - 1);
			M_T = stoi(kk3);

		}
		if (str.find("Quantile", index) != str.npos)
		{
			size_t kk = str.find(":");
			size_t pos = str.find(",");
			string kk3 = str.substr(kk + 1, pos - kk - 1);
			M_Quantile = stod(kk3);
		}


		if (str.find("Alpha", index) != str.npos)
		{
			size_t kk = str.find(":");
			size_t pos = str.find(",");
			string kk3 = str.substr(kk + 1, pos - kk - 1);
			M_Alpha = stod(kk3);
		}
		if (str.find("ComputationTime", index) != str.npos)
		{
			size_t kk = str.find(":");
			size_t pos = str.find(",");
			string kk3 = str.substr(kk + 1, pos - kk - 1);
			M_ComputionTime = stoi(kk3);
		}
		if (str.find("Scenarios_number", index) != str.npos)
		{
			int temp;
			M_Total_Scenarios = 0;
			for (int i = 0; i < M_T; i++)
			{
				getline(ifs, str);
				size_t pos = str.find(",");
				temp = stoi(str.substr(1, pos - 1));
				if (temp > 1)
					M_Pool = true;
				M_Total_Scenarios += temp;
				M_Scenarios_number.push_back(temp);

			}


		}
	}
	for (int i = 0; i < M_T; i++)
	{
		M_T_Season.push_back(-1);
	}
	//	settingsFile.seekg(0, settingsFile.beg);
	ifs.clear();
	ifs.seekg(0, ifs.beg);
	string name;
	//Seasons

	bool bSeason = false;
	while (getline(ifs, str) && bSeason == false)
	{
		size_t index = 0;

		if (str.find("Seasons\":{", index) != str.npos)
		{
			bSeason = true;
			int cont = 0;
			do
			{
				getline(ifs, str);
				//Empieza una season
				if (str.find(":[", 0) != str.npos)
				{
					name = str.substr(str.find("\"", 0) + 1, str.find(":[", 0) - str.find("\"", 0) - 2);
					M_S_Id.insert(pair<string, int>(name, cont));
					M_S_Name.push_back(name);
					Season S(cont);

					S.Set_Name(name);
					vector<int> numbers;
					do
					{

						getline(ifs, str);
						if (str.size() != 0)
						{

							if (str.find("],") == str.npos && (str.find("]") == str.npos))
							{
								name = str.substr(str.find("\"", 0) + 1, str.find(",", 0) - str.find("\"", 0) - 2);
								numbers.push_back(stoi(name));
							}
						}

					} while (str.find("],") == str.npos && (str.find("]") == str.npos));
					S.Set_Numbers(numbers);

					for (vector<int>::iterator it2 = S.Get_Numbers().begin(); it2 != S.Get_Numbers().end(); it2++)
					{
						M_T_Season[(*it2) - 1] = cont;
						//						M_T_Season[(*it2) ] = cont;

					}
					M_Seasons.push_back(S);
					cont++;

				}
			} while (str.find("],") != str.npos && (str.find("]") != str.npos));
		}
	}


	//	settingsFile.seekg(0, settingsFile.beg);
	ifs.clear();
	ifs.seekg(0, ifs.beg);
	//Resources
	bool bResources = false;
	while (getline(ifs, str) && bResources == false)
	{
		size_t index = 0;

		if (str.find("Resources", index) != str.npos)
		{
			bResources = true;
			int cont = 0;
			do
			{
				getline(ifs, str);
				//Empieza una season
				if (str.find(":{", 0) != str.npos)
				{
					name = str.substr(str.find("\"", 0) + 1, str.find(":{", 0) - str.find("\"", 0) - 2);

					Resource R(cont);
					R.Set_Name(name);
					vector<double> vec(M_T);
					R.Set_Temp(vec);
					R.Set_Copy(vec);
					M_R_Name.push_back(name);
					M_R_Id.insert(pair<string, int>(name, cont));

					vector<double> min;
					vector<double> max;
					getline(ifs, str); //lee min
					double temp;
					bool abajo = false;
					for (int i = 0; i < M_T; i++)
					{
						getline(ifs, str);
						size_t pos = str.find(",");
						temp = stod(str.substr(1, pos - 1));
						if (temp > __FLT_EPSILON__)
						{
							M_Lower_Resources = true;
							abajo = true;
						}
						min.push_back(temp);

					}
					getline(ifs, str); //lee el fin de la anterior
					getline(ifs, str); //lee max
					for (int i = 0; i < M_T; i++)
					{
						getline(ifs, str);
						size_t pos = str.find(",");
						temp = stod(str.substr(1, pos - 1));
						max.push_back(temp);

					}
					R.Set_Lower(min);
					R.Set_Upper(max);

					M_Lower_ResourcesV.push_back(abajo);
					M_Resources.push_back(R);
					M_N_Resources++;
					cont++;



				}
				getline(ifs, str); //lee el ]
				getline(ifs, str);
			} while (str.find("},") != str.npos);
		}

	}
	//leidos resources
	ifs.clear();
	ifs.seekg(0, ifs.beg);
	while (getline(ifs, str))
	{
		size_t index = 0;

		if (str.find("Interventions", index) != str.npos)
		{
			int cont = 0;
			do
			{
				getline(ifs, str);
				//Empieza una intervention
				if (str.find(":{", 0) != str.npos)
				{
					name = str.substr(str.find("\"", 0) + 1, str.find(":{", 0) - str.find("\"", 0) - 2);
					Intervention I(cont);
					I.Set_Name(name);
					M_I_Id.insert(pair<string, int>(name, cont));
					M_I_Name.push_back(name);
					cont++;
					getline(ifs, str);
					string kk = str.substr(str.find(":", 0) + 1, str.find(",", 0) - str.find(":", 0) - 2);
					I.Set_Tmax(stoi(str.substr(str.find(":", 0) + 2, str.find(",", 0) - str.find(":", 0) - 3)));
					getline(ifs, str);//ahora lee los deltas
					vector<int> deltas;
					int temp_val;
					for (int i = 0; i < M_T; i++)
					{
						getline(ifs, str);
						size_t pos = str.find(",");
						temp_val = stoi(str.substr(1, pos - 1));
						deltas.push_back(temp_val);
					}
					I.Set_Delta(deltas);
					I.Set_Min_Delta(*min_element(I.Get_Delta().begin(), I.Get_Delta().end()));
					getline(ifs, str);//ahora lee los workload
					getline(ifs, str);
					//Empieza los workload
					map<int, map<int, map<int, double >>>  workload;
					do
					{
						getline(ifs, str);
						//Empieza un recurso
						if (str.find(":{", 0) != str.npos)
						{

							string resource = str.substr(str.find("\"", 0) + 1, str.find(":{", 0) - str.find("\"", 0) - 2);
							map<int, map<int, double >> work_res;


							do
							{
								getline(ifs, str);
								//Empieza una season
								if (str.find(":{", 0) != str.npos)
								{


									string tactual = str.substr(str.find("\"", 0) + 1, str.find(":{", 0) - str.find("\"", 0) - 2);
									map<int, double >  work_sea;

									do
									{
										getline(ifs, str);
										if (str.find("}", 0) == str.npos)
										{

											string tpast = str.substr(str.find("\"", 0) + 1, str.find(":", 0) - str.find("\"", 0) - 2);
											string val_str = str.substr(str.find(":", 0) + 1, str.find(",", 0) - str.find(":", 0) - 1);
											double val = stod(str.substr(str.find(":", 0) + 1, str.find(",", 0) - str.find(":", 0) - 1));

											work_sea.insert(pair<int, double >(atoi(tpast.c_str()) - 1, val));



										}

									} while (str.find("}") == str.npos);
									work_res.insert(pair<int, map<int, double >>(atoi(tactual.c_str()) - 1, work_sea));


								}

							} while (str.find("},") != str.npos);
							getline(ifs, str);
							workload.insert(pair<int, map<int, map<int, double >>>(M_R_Id[resource], work_res));

						}



					} while (str.find("},") != str.npos);
					vector<vector<vector<double>>> workload_N;


					//Poner en vector
					for (size_t i = 0; i < M_N_Resources; i++)
					{
						vector<vector<double> > workload_N2;
						for (int t = 0; t < I.Get_Tmax(); t++)
						{
							vector<double> workload_N3;
							for (int t2 = t; t2 < t + I.Get_Delta(t); t2++)
							{
								if ((workload.find(i) != workload.end())
									&& (workload[i].find(t2) != workload[i].end())
									&& (workload[i][t2].find(t) != workload[i][t2].end()))

									workload_N3.push_back(workload[i][t2][t]);
								else
									workload_N3.push_back(0);

							}
							workload_N2.push_back(workload_N3);
						}
						workload_N.push_back(workload_N2);


					}
					I.Set_Workload(workload);
					I.Set_WorkloadN(workload_N);

					//Accumulative workload
					vector<double > vec_workloadC;
					for (int ik = 0; ik < M_T; ik++)
					{
						vec_workloadC.push_back(0);

					}
					for (map<int, map<int, map<int, double >>>::iterator itw = workload.begin(); itw != workload.end(); itw++)
					{

						for (map<int, map<int, double >>::iterator itw2 = itw->second.begin(); itw2 != itw->second.end(); itw2++)
						{

							for (map<int, double> ::iterator itw3 = itw2->second.begin(); itw3 != itw2->second.end(); itw3++)
							{

								vec_workloadC[itw3->first] += ((itw3->second) / (M_Resources[(*itw).first].Get_Upper()[(*itw2).first]));
							}
						}
					}
					//					for (int ik = 0; ik < M_T; ik++)
					vector <double> tempWxt(vec_workloadC.size());
					for (size_t i = 0; i < vec_workloadC.size(); i++)
					{
						tempWxt[i] = vec_workloadC[i] / I.Get_Delta()[i];
					}
					I.Set_WorkloadCxT(tempWxt);
					I.Set_WorkloadC(vec_workloadC);
					I.Set_Min_WorkloadxT(tempWxt[Min(tempWxt)]);
					I.Set_Max_WorkloadxT(tempWxt[Max(tempWxt)]);
					I.Set_Min_Workload(vec_workloadC[Min(vec_workloadC)]);
					I.Set_Max_Workload(vec_workloadC[Max(vec_workloadC)]);

					//Empieza los risk
					//TODO
					getline(ifs, str);
					getline(ifs, str);
					map<int, map<int, vector< double > >> risk;
					do
					{
						getline(ifs, str);
						//Empieza un recurso
						if (str.find(":{", 0) != str.npos)
						{

							string tactual = str.substr(str.find("\"", 0) + 1, str.find(":{", 0) - str.find("\"", 0) - 2);


							map<int, vector< double > > risk_actual;

							do
							{

								getline(ifs, str);
								//Empieza una season
								if (str.find(":[", 0) != str.npos)
								{


									string tinicio = str.substr(str.find("\"", 0) + 1, str.find(":[", 0) - str.find("\"", 0) - 2);

									double temp2;
									vector<double> risk_scenarios;
									do
									{
										getline(ifs, str);
										size_t pos = str.find(",");
										temp2 = stod(str.substr(1, pos - 1));
										risk_scenarios.push_back(temp2);
									} while (str.find(",") != str.npos);

									risk_actual.insert(pair<int, vector< double> >(atoi(tinicio.c_str()) - 1, risk_scenarios));

									getline(ifs, str);
								}

							} while (str.find("]") != str.npos);

							risk.insert(pair<int, map<int, vector<double>  >>(atoi(tactual.c_str()) - 1, risk_actual));



						}



					} while (str.find("},") != str.npos);
					vector<vector<vector<double>>> riskN;
					//					vector<vector<double>> risk_actualN;
					//					vector<double> risk_scenarios;

										//Riesgo acumulado por día de inicio
					vector<double > vec_riskC(M_T);
					vector<double > vec_riskCxT(M_T);
					vector<double> vec_riskQuantile(M_T);
					vector<double> vec_riskExcess(M_T);
					vector<vector<double>> mat_riskexcess;
					//			vector<double> vec_riskMC;

					//Poner en vector
					for (int ta = 0; ta < I.Get_Tmax(); ta++)
					{
						vector<vector<double>> risk_actualN;
						vector<double> vec_riskexcess(deltas[ta]);
						for (int ta2 = ta; ta2 < ta + deltas[ta]; ta2++)
						{

							vector<double> risk_scenarios;
							double acumulado = 0;
							vector<double> temp;
							for (int t2 = 0; t2 < M_Scenarios_number[ta2]; t2++)
							{
								if ((risk.find(ta2) != risk.end())
									&& (risk[ta2].find(ta) != risk[ta2].end()))
									risk_scenarios.push_back(risk[ta2][ta][t2]);// [ta2] [ta] [t2] );
								else
									risk_scenarios.push_back(0);
								acumulado += risk_scenarios[t2];
								temp.push_back(risk_scenarios[t2]);
							}
							double media = acumulado / M_Scenarios_number[ta2];
							vec_riskC[ta] += acumulado / M_Scenarios_number[ta2];
							if (M_Scenarios_number[ta2] > 1)
							{
								int pos = ceil(M_Scenarios_number[ta2] * M_Quantile) - 1;


								nth_element(temp.begin(), temp.begin() + pos, temp.end());
								//		double cuantil1= M_T_Temp_Risk_S[i][pos];
								//		 sort(M_T_Temp_Risk_S[i].begin(), M_T_Temp_Risk_S[i].end());
								vec_riskQuantile[ta] += temp[pos];
								vec_riskExcess[ta] += max((double)0, (temp[pos] - media));
								vec_riskexcess[ta2 - ta] = max((double)0, (temp[pos] - media));
								if (vec_riskexcess[ta2 - ta] < -1 * __FLT_EPSILON__)
									Problema(111);
							}
							else
							{
								vec_riskQuantile[ta] = vec_riskC[ta];
								vec_riskExcess[ta] = 0;
								vec_riskexcess[ta2 - ta] = 0;
							}
							risk_actualN.push_back(risk_scenarios);

						}
						mat_riskexcess.push_back(vec_riskexcess);
						riskN.push_back(risk_actualN);


					}
					//					I.Set_Workload(workload);
					I.Set_RiskExcessXt(mat_riskexcess);
					I.Set_Risk(risk);
					I.Set_RiskN(riskN);

					/*

							//			vector<double> vec_riskMC;
										for (int ik = 0; ik < M_T; ik++)
										{
											vec_riskC.push_back(0);
											vec_riskCxT.push_back(0);
											vec_riskQuantile.push_back(0);
											vec_riskExcess.push_back(0);


										}
										for (map<int, map<int, vector< double > >>::iterator itm = risk.begin(); itm != risk.end(); itm++)
										{


											for (map<int, vector< double > >::iterator it2m = itm->second.begin(); it2m != itm->second.end(); it2m++)
											{
												double acumulado = 0;
												vector<double> temp;
					//							temp.push_back(accumulate(temp.begin(), temp.end(), 0.0));
												for (int si = 0; si < it2m->second.size(); si++)
												{
													 acumulado+= it2m->second[si];
													 temp.push_back(it2m->second[si]);
												}
										//		vec_riskC[it2m->first] += acumulado;
												double media= acumulado / it2m->second.size();
												vec_riskC[it2m->first] += acumulado / it2m->second.size();
												if (it2m->second.size() >1)
												{
													int pos = ceil(it2m->second.size() * M_Quantile) - 1;


													nth_element(temp.begin(), temp.begin() + pos, temp.end());
													//		double cuantil1= M_T_Temp_Risk_S[i][pos];
													//		 sort(M_T_Temp_Risk_S[i].begin(), M_T_Temp_Risk_S[i].end());
													vec_riskQuantile[it2m->first] += temp[pos];
													vec_riskExcess[it2m->first] += temp[pos] - media;
												}
												else
												{
													vec_riskQuantile[it2m->first] = vec_riskC[it2m->first];
													vec_riskExcess[it2m->first] = 0;
												}

											}

					//						temp.push_back(accumulate(temp.begin(), temp.end(), 0.0));

										}
						*/
					for (size_t i = 0; i < vec_riskCxT.size(); i++)
					{
						vec_riskCxT[i] = vec_riskC[i] / I.Get_Delta()[i];
					}
					I.Set_RiskExcess(vec_riskExcess);
					I.Set_RiskC(vec_riskC);
					I.Set_RiskCxT(vec_riskCxT);
					I.Set_RiskCQuantile(vec_riskQuantile);
					I.Set_Min_RiskxT(vec_riskCxT[Min(vec_riskCxT)]);
					I.Set_Max_RiskxT(vec_riskCxT[Max(vec_riskCxT)]);

					I.Set_Max_Risk(vec_riskC[Max(vec_riskC)]);
					I.Set_Max_RiskQ(vec_riskQuantile[Max(vec_riskQuantile)]);
					vector<int> T_ordenados_Risk;
					vector<int> T_ordenados_RiskQ;
					//En T ordenados deben estar todos los que entran
					//hay algunos que entran solos
					for (int i1 = 0; i1 < (I.Get_Tmax()); i1++)
					{
						if (PuedoPonerSola(I, i1) == false) continue;
						bool colocado1 = false;
						for (int i2 = 0; i2 < T_ordenados_Risk.size() && colocado1 == false; i2++)
						{
							if (vec_riskC[i1] < (vec_riskC[T_ordenados_Risk[i2]] - __FLT_EPSILON__))
							{
								T_ordenados_Risk.insert(T_ordenados_Risk.begin() + i2, i1);
								colocado1 = true;
							}
						}

						if (colocado1 == false)
							T_ordenados_Risk.push_back(i1);
						bool colocado2 = false;
						for (int i2 = 0; i2 < T_ordenados_RiskQ.size() && colocado2 == false; i2++)
						{
							if (vec_riskQuantile[i1] < (vec_riskQuantile[T_ordenados_RiskQ[i2]] - __FLT_EPSILON__))
							{
								T_ordenados_RiskQ.insert(T_ordenados_RiskQ.begin() + i2, i1);
								colocado2 = true;
							}
						}

						if (colocado2 == false)
							T_ordenados_RiskQ.push_back(i1);
					}
					I.Set_Min_Risk(vec_riskC[T_ordenados_Risk[0]]);
					I.Set_Min_RiskQ(vec_riskQuantile[T_ordenados_RiskQ[0]]);
					if (T_ordenados_Risk.size() > 1)
						I.Set_Regret(vec_riskC[T_ordenados_Risk[1]] - vec_riskC[T_ordenados_Risk[0]]);
					else
						I.Set_Regret(0);
					if (T_ordenados_RiskQ.size() > 1)
						I.Set_RegretQ(vec_riskQuantile[T_ordenados_RiskQ[1]] - vec_riskQuantile[T_ordenados_RiskQ[0]]);
					else
						I.Set_RegretQ(0);
					//					I.Set_Max_Risk(vec_riskC[T_ordenados_Risk[T_ordenados_Risk.size()-1]]);
					double val1 = vec_riskC[T_ordenados_Risk[0]];
					list<int> T_ordenados_Risk_I;
					for (int i = 1; i < T_ordenados_Risk.size(); i++)
					{
						if (vec_riskC[T_ordenados_Risk[i]] > (val1 + __FLT_EPSILON__))
						{
							val1 = vec_riskC[T_ordenados_Risk[i]];
							T_ordenados_Risk_I.push_back(i);
						}
					}
					T_ordenados_Risk_I.push_back(T_ordenados_Risk.size());
					val1 = vec_riskQuantile[T_ordenados_RiskQ[0]];
					list<int> T_ordenados_Risk_IQ;
					for (int i = 1; i < T_ordenados_RiskQ.size(); i++)
					{
						if (vec_riskQuantile[T_ordenados_RiskQ[i]] > (val1 + __FLT_EPSILON__))
						{
							val1 = vec_riskQuantile[T_ordenados_RiskQ[i]];
							T_ordenados_Risk_IQ.push_back(i);
						}
					}
					T_ordenados_Risk_IQ.push_back(T_ordenados_RiskQ.size());
					I.Set_T_ordenados_Risk_IQ(T_ordenados_Risk_IQ);
					I.Set_T_ordenados_RiskQ(T_ordenados_RiskQ);
					I.Set_T_ordenados_Risk_I(T_ordenados_Risk_I);
					I.Set_T_ordenados_Risk(T_ordenados_Risk);
					M_N_Interventions = cont;
					M_Interventions.push_back(I);
					//					cont++;


				}
				getline(ifs, str);
				getline(ifs, str);
			} while (str.find("},") != str.npos && (str.find("}") != str.npos));

		}
	}
	/*		if (str.find("Seasons", index) != str.npos)
			{
				int kk = 9;
			}
			if (str.find("Interventions", index) != str.npos)
			{
				int kk = 9;
			}
			*/
			//Ahora Exclusions
				//leidos resources
	ifs.clear();
	ifs.seekg(0, ifs.beg);
	bool bexclusions = false;

	while (getline(ifs, str) && bexclusions == false)
	{
		size_t index = 0;
		if (str.find("Exclusions", index) != str.npos)
		{
			bexclusions = true;

			do
			{
				getline(ifs, str);
				//Empieza una season
				if (str.find(":[", 0) != str.npos)
				{
					vector <int> exclusion_int;
					vector <string> exclusion;
					getline(ifs, str);
					name = str.substr(str.find("\"", 0) + 1, str.find("\"", str.find("\"", 0) + 1) - str.find("\"", 0) - 1);

					exclusion.push_back(name);
					getline(ifs, str);
					string name2 = str.substr(str.find("\"", 0) + 1, str.find("\"", str.find("\"", 0) + 1) - str.find("\"", 0) - 1);
					exclusion.push_back(name2);
					getline(ifs, str);
					string name3 = str.substr(str.find("\"", 0) + 1, str.find("\"", str.find("\"", 0) + 1) - str.find("\"", 0) - 1);
					exclusion.push_back(name3);

					M_Exclusions_Originals.push_back(exclusion);
					exclusion_int.push_back(M_I_Id[exclusion[0]]);
					exclusion_int.push_back(M_I_Id[exclusion[1]]);
					exclusion_int.push_back(M_S_Id[exclusion[2]]);
					M_Interventions[M_I_Id[exclusion[0]]].Add_Exclusion(M_I_Id[exclusion[1]], M_S_Id[exclusion[2]]);
					M_Interventions[M_I_Id[exclusion[1]]].Add_Exclusion(M_I_Id[exclusion[0]], M_S_Id[exclusion[2]]);
					//Convert string to int
					M_Exclusions.push_back(exclusion_int);
					M_N_Exclusions++;
				}
				getline(ifs, str);
			} while (str.find("],") != str.npos);

		}
	}
	/*		while (true) {

				index = str.find(":{", index);
				if (index == string::npos) break;
				str.replace(index, 3, 1, '-');
				index += 1;

			}*/

			/*
			json j = json::parse(ifs);
			//T
			if (j.find("T") != j.end()) {
				json info = j["T"];
				M_T = info;

			}
			//Quantile
			if (j.find("Quantile") != j.end()) {
				json info = j["Quantile"];
				M_Quantile = info;
			}
			//Alpha
			if (j.find("Alpha") != j.end()) {
				json info = j["Alpha"];
				M_Alpha = info;
			}
			cout << M_T << M_Quantile << M_Alpha;
			M_T_Season.clear();
			for (int i = 0; i < M_T; i++)
				M_T_Season.push_back(-1);
			// Seasons

			if (j.find("Scenarios_number") != j.end()) {

				M_Scenarios_number = j["Scenarios_number"].get<vector<int>>();
			}

			if (j.find("Seasons") != j.end())
			{
				json info = j["Seasons"];
				size_t i=0;
				for (json::iterator it = info.begin(); it != info.end(); ++it)
				{
					M_S_Id.insert(pair<string, int>(it.key(), i));
					M_S_Name.push_back(it.key());
					Season S(i);

					S.Set_Name(it.key());

					if ((*it).is_number())
						S.Set_Numbers((*it).get<vector<int>>());
					else
					{
						vector <string> kk = (*it);
						vector < int> intNumbers;
						for (int ik = 0; ik < kk.size(); ik++)
						{
							int num = atoi(kk.at(ik).c_str());
							intNumbers.push_back(num);
						}
						S.Set_Numbers(intNumbers);
					}
					for (vector<int>::iterator it2 = S.Get_Numbers().begin(); it2 != S.Get_Numbers().end(); it2++)
					{
						M_T_Season[(*it2) - 1] = i;
					}
					M_Seasons.push_back(S);
					i++;
				}
			}
			// Resources


			if (j.find("Resources") != j.end())
			{
				json info = j["Resources"];

				// verify that "name" attribute exists in info
				size_t i=0;
				for (json::iterator it = info.begin(); it != info.end(); ++it)
				{
					//           string kk = it.key().substr(11, it.key().size() - 11);
							   //            exclusion_int.push_back(atoi(kk.c_str()));
					Resource R(i);
					R.Set_Name(it.key());
					vector<double> vec(M_T);
					R.Set_Temp(vec);
					M_R_Name.push_back(it.key());
					M_R_Id.insert(pair<string, int>(it.key(), i));

					if ((*it).find("max") != (*it).end())
					{
						R.Set_Upper((*it)["max"].get<vector<double>>());
						//              cout << (*it)["max"]; // upper

					}
					if ((*it).find("min") != (*it).end())
					{
						R.Set_Lower((*it)["min"].get<vector<double>>());
						//                cout << (*it)["min"]; // upper

					}
					M_Resources.push_back(R);
					M_N_Resources++;
					i++;



				}
			}

			// Interventions

			if (j.find("Interventions") != j.end())
			{
				json info = j["Interventions"];
				int sizei = info.size();
				cout << "número de interventions" << sizei << endl;
				// verify that "name" attribute exists in info
				size_t i=0;
				for (json::iterator it = info.begin(); it != info.end(); ++it)
				{
					Intervention I(i);
					I.Set_Name(it.key());
					M_I_Id.insert(pair<string, int>(it.key(), i));
					M_I_Name.push_back(it.key());
					//           string kk = it.key().substr(13, it.key().size() - 13);
				   //            exclusion_int.push_back(atoi(kk.c_str()));


					if ((*it).find("tmax") != (*it).end())
					{
						if ((*it)["tmax"].is_number())
						{
							I.Set_Tmax((*it)["tmax"] - 1);
						}
						else
						{
							string  kk = (*it)["tmax"];
							I.Set_Tmax(atoi(kk.c_str()) - 1);
						}

						//               cout << I.t_max; // upper


					}
					if ((*it).find("Delta") != (*it).end())
					{
						I.Set_Delta((*it)["Delta"].get<vector<int>>());
						//              cout << (*it)["Delta"]; // upper
						I.Set_Min_Delta(*min_element(I.Get_Delta().begin(), I.Get_Delta().end()));

					}
					if ((*it).find("workload") != (*it).end())
					{
						map<int, map<int, map<int, double >>>  workload;
						//           vector<int> workload_r;
						json info3 = (*it)["workload"];
						for (json::iterator it2 = info3.begin(); it2 != info3.end(); ++it2)
						{
							//                 workload_r.push_back(M_R_Id[it2.key()]);
							map<int, map<int, double >> work_res;
							//Resource
							json info4 = info3[it2.key()];
							for (json::iterator it3 = info4.begin(); it3 != info4.end(); ++it3)
							{
								//Time  current
								map<int, double >  work_sea;
								json info5 = info4[it3.key()];
								for (json::iterator it4 = info5.begin(); it4 != info5.end(); ++it4)
								{
									//Time ini
									json info6 = info5[it4.key()];
									work_sea.insert(pair<int, double >(atoi(it4.key().c_str()) - 1, info6));



								}
								work_res.insert(pair<int, map<int, double >>(atoi(it3.key().c_str()) - 1, work_sea));

							}
							workload.insert(pair<int, map<int, map<int, double >>>(M_R_Id[it2.key()], work_res));
							I.Set_Workload(workload);
							//                   I.Set_WorkloadR(workload_r);


											//   I.Set_WorkloadT(workload_r);

						}

						//Accumulative workload
						vector<double > vec_workloadC;
						for (int ik = 0; ik < M_T; ik++)
						{
							vec_workloadC.push_back(0);

						}
						for (map<int, map<int, map<int, double >>>::iterator itw = workload.begin(); itw != workload.end(); itw++)
						{

							for (map<int, map<int, double >>::iterator itw2 = itw->second.begin(); itw2 != itw->second.end(); itw2++)
							{

								for (map<int, double> ::iterator itw3 = itw2->second.begin(); itw3 != itw2->second.end(); itw3++)
								{

									vec_workloadC[itw3->first] += itw3->second;
								}
							}
						}

						I.Set_WorkloadC(vec_workloadC);
						I.Set_Min_Workload(vec_workloadC[Min(vec_workloadC)]);
						I.Set_Max_Workload(vec_workloadC[Max(vec_workloadC)]);
					}
					if ((*it).find("risk") != (*it).end())
					{
						map<int, map<int, vector< double > >> risk;
						json info3 = (*it)["risk"];
						for (json::iterator it2 = info3.begin(); it2 != info3.end(); ++it2)
						{
							//SEason
							map<int, vector< double > > risk_sea;
							json info4 = info3[it2.key()];
							for (json::iterator it3 = info4.begin(); it3 != info4.end(); ++it3)
							{

								//Time
								json info5 = info4[it3.key()];
								//Scenarios
								vector<double> temp = info5.get<vector<double>>();

								temp.push_back(accumulate(temp.begin(), temp.end(), 0.0));

								risk_sea.insert(pair<int, vector<double >>(atoi(it3.key().c_str()) - 1, temp));

							}
							risk.insert(pair<int, map<int, vector<double >>>(atoi(it2.key().c_str()) - 1, risk_sea));
						}
						I.Set_Risk(risk);

						//Riesgo acumulado por día de inicio
						vector<double > vec_riskC;
						for (int ik = 0; ik < M_T; ik++)
						{
							vec_riskC.push_back(0);

						}
						for (map<int, map<int, vector< double > >>::iterator itm = risk.begin(); itm != risk.end(); itm++)
						{


							for (map<int, vector< double > >::iterator it2m = itm->second.begin(); it2m != itm->second.end(); it2m++)
							{
								vec_riskC[it2m->first] += it2m->second[it2m->second.size() - 1];
							}

						}
						I.Set_RiskC(vec_riskC);
						I.Set_Min_Risk(vec_riskC[Min(vec_riskC)]);
						I.Set_Max_Risk(vec_riskC[Max(vec_riskC)]);
						vector<int> T_ordenados_Risk;
						for (int i1 = 0; i1 < (I.Get_Tmax()); i1++)
						{
							bool colocado = false;
							for (int i2 = 0; i2 < T_ordenados_Risk.size() && colocado == false; i2++)
							{
								if (vec_riskC[i1] < (vec_riskC[T_ordenados_Risk[i2]] - __FLT_EPSILON__))
								{
									T_ordenados_Risk.insert(T_ordenados_Risk.begin() + i2, i1);
									colocado = true;
								}
							}
							if (colocado == false)
								T_ordenados_Risk.push_back(i1);
						}
						I.Set_T_ordenados_Risk(T_ordenados_Risk);



					}
					M_N_Interventions = i;
					M_Interventions.push_back(I);
					i++;
				}
			}




			//Exclusion
			if (j.find("Exclusions") != j.end())
			{
				json info = j["Exclusions"];
				int sizei = info.size();
				//      cout << sizei << endl;
				for (json::iterator it = info.begin(); it != info.end(); ++it)
				{
					vector <int> exclusion_int;
					vector <string> exclusion = info[it.key()];

					M_Exclusions_Originals.push_back(exclusion);
					exclusion_int.push_back(M_I_Id[exclusion[0]]);
					exclusion_int.push_back(M_I_Id[exclusion[1]]);
					exclusion_int.push_back(M_S_Id[exclusion[2]]);
					M_Interventions[M_I_Id[exclusion[0]]].Add_Exclusion(M_I_Id[exclusion[1]], M_S_Id[exclusion[2]]);
					M_Interventions[M_I_Id[exclusion[1]]].Add_Exclusion(M_I_Id[exclusion[0]], M_S_Id[exclusion[2]]);
					//Convert string to int
					M_Exclusions.push_back(exclusion_int);
					M_N_Exclusions++;
				}

			}
			*/

}

void Maintenance::PrintPSolutionStatistics( vector<int>& lista)
{
	//	ofstream file(name, ios_base::binary | ios_base::out);
	LimpiarVectores();

	for (int i=0;i<M_N_Interventions;i++)//list<pair<int, int>>::iterator it = lista.begin(); it != lista.end(); it++)
	{
		//		ColocarEnTiempo((*it).first, (*it).second);
		cout << M_Interventions[i].Get_Name() << " " << lista[i] + 1 << endl;
	}
	cout << endl;


	for (int i = 0; i < M_N_Interventions; i++)//list<pair<int, int>>::iterator it = lista.begin(); it != lista.end(); it++)
	{
		ColocarEnTiempo(i, lista[i]);
		//		file << M_Interventions[(*it).first].Get_Name() << " " << (*it).second + 1 << endl;
	}

	for (size_t i=0; i < M_N_Interventions; i++)
	{
		cout << M_Interventions_T[i] << " ";
	}
	cout << endl;
	PrintValores();


}
void Maintenance::PrintSolution(string& name, vector<int> &lista)
{
	//	ofstream file(name, ios_base::binary | ios_base::out);
	ofstream file;
	file.open(name);
	for (int i=0;i<M_N_Interventions;i++)//list<pair<int, int>>::iterator it = lista.begin(); it != lista.end(); it++)
	{
		file << M_Interventions[i].Get_Name() << " " << lista[i] + 1 << endl;

	}
	//	file << endl;
	file.close();

}

void Maintenance::PrintTimes(string &name)
{
    ofstream file;
    string name2=name + "_sum.txt";
    file.open(name2);
    file << endl<< "Instancia"<<endl;
    for (int i = 0; i < M_T; i++)
    {
        file << M_T_Season[i] << " ";
    }
    file << endl;
	for (int i = 0; i < M_T; i++)
	{
		file << M_Scenarios_number[i] << " ";
	}
	file << endl;
    for (size_t j = 0; j < M_Resources.size(); j++)
    {
        file << endl<<M_R_Name[j] << endl;
        for (int i = 0; i < M_T; i++)
        {

            file << M_Resources[j].Get_Lower()[i] << " ";
        }
        file << endl;
        for (int i = 0; i < M_T; i++)
        {

            file << M_Resources[j].Get_Upper()[i] << " ";
        }
    }
	for (int j = 0; j < M_Interventions.size(); j++)
	{
		file << endl << M_I_Name[j] << " Workload: Min Max " << M_Interventions[j].Get_Min_Workload() << " " << M_Interventions[j].Get_Max_Workload() << " Risk: Min Max " << M_Interventions[j].Get_Min_Risk() << " " << M_Interventions[j].Get_Max_Risk() << endl;
		for (int i = 0; i < M_T; i++)
		{

			file << M_Interventions[j].Get_Delta()[i] << " ";
		}
		file << endl << "Risk Acumulado" << endl;
		for (int i = 0; i < M_T; i++)
		{

			file << M_Interventions[j].Get_RiskC(i) << " ";
		}
		file << endl << "Workload Acumulado" << endl;
		for (int i = 0; i < M_T; i++)
		{

			file << M_Interventions[j].Get_WorkloadC(i) << " ";
		}

	}
	file << endl<<"Exclusions"<<endl;
	for (list<vector<string> >::iterator it=M_Exclusions_Originals.begin(); it != M_Exclusions_Originals.end(); it++)
	{
		file << (*it)[0] << " " << (*it)[1] << " " << (*it)[2] <<endl;
	}
    file.close();
}
void Maintenance::PrintResourcesT(int tiempo)
{
	cout << endl;
	for (list<pair<int, int>> ::iterator it = M_I_T_Solution.begin(); it != M_I_T_Solution.end(); it++)
	{


		for (int i2 = 0; i2 < M_Interventions[(*it).first].Get_Workload_Res().size(); i2++)
		{
			for (int tprima = (*it).second; tprima < ((*it).second + M_Interventions[(*it).first].Get_Delta()[(*it).second]); tprima++)
			{
				if (tprima != tiempo) continue;
				if (M_Interventions[(*it).first].Get_Workload()[i2][tprima][tiempo] <= 0) continue;

							cout << M_I_Name[(*it).first] << " " << tprima << " " << (*it).second << " " << M_R_Name[i2]<< " " << M_Interventions[(*it).first].Get_Workload()[i2][tprima][tiempo]<<endl;
			}
		}
	}

}
void Maintenance::PrintResources()
{
	cout << endl;
	for (int j = 0; j < M_Resources.size(); j++)
	{
		cout <<endl<< M_R_Name[j] << endl;
		for (int i = 0; i < M_T; i++)
		{

			cout << M_Resources[j].Get_Lower()[i] << " ";
		}
		cout << endl;
		for (int i = 0; i < M_T; i++)
		{

			cout << M_Resources[j].Get_Upper()[i] << " ";
		}
		cout << endl;
		for (int i = 0; i < M_T; i++)
		{

			cout << M_Resources[j].Get_Temp()[i] << " ";
		}
	}


}
void Maintenance::PrintValores()
{
	cout << endl;
	for (int i = 0; i < M_T; i++)
	{
		cout << M_T_Season[i] << " ";
	}
	cout << endl;
	for (int i = 0; i < M_T; i++)
	{
		cout << M_Scenarios_number[i] << " ";
	}
	cout << endl;
	for (int j = 0; j < M_Resources.size(); j++)
	{
		cout << endl << M_R_Name[j] << endl;
		for (int i = 0; i < M_T; i++)
		{

			cout << i << " ";
		}
		cout << endl;
		for (int i = 0; i < M_T; i++)
		{

			cout << M_Resources[j].Get_Lower()[i] << " ";
		}
		cout << endl;
		for (int i = 0; i < M_T; i++)
		{

			cout << M_Resources[j].Get_Upper()[i] << " ";
		}
		cout << endl;
		for (int i = 0; i < M_T; i++)
		{

			cout << M_Resources[j].Get_Temp()[i] << " ";
		}

	}
	for (int j = 0; j < M_Interventions.size(); j++)
	{
		cout << endl << M_I_Name[j] << " Workload: Min Max " << M_Interventions[j].Get_Min_Workload() << " " << M_Interventions[j].Get_Max_Workload() << " Risk: Min Max " << M_Interventions[j].Get_Min_Risk() << " " << M_Interventions[j].Get_Max_Risk() << endl;
		for (int i = 0; i < M_T; i++)
		{

			cout << M_Interventions[j].Get_Delta()[i] << " ";
		}
		cout << endl << "Risk Acumulado" << endl;
		for (int i = 0; i < M_T; i++)
		{

			cout << M_Interventions[j].Get_RiskC(i) << " ";
		}
		cout << endl << "Workload Acumulado" << endl;
		for (int i = 0; i < M_T; i++)
		{

			cout << M_Interventions[j].Get_WorkloadC(i) << " ";
		}

	}
	cout << endl << "Exclusions" << endl;
	for (list<vector<string> >::iterator it = M_Exclusions_Originals.begin(); it != M_Exclusions_Originals.end(); it++)
	{
		cout << (*it)[0] << " " << (*it)[1] << " " << (*it)[2] << endl;
	}

}
/*
void Maintenance::ChapuzaDiversidad(string& file_to_read)
{

	FILE *read;       

	read = fopen(file_to_read.c_str(), "r");
	if (read == NULL)
	{
		fprintf(stderr, "Can't open input file %s!\n", file_to_read.c_str());
		exit(0);
	}
	string file_to_write = file_to_read + "n100_m10.txt";
	FILE* write;       

	write = fopen(file_to_write.c_str(), "w");
	if (write == NULL)
	{
		fprintf(stderr, "Can't open input file %s!\n", file_to_read.c_str());
		exit(0);
	}
	int n, m;


	fscanf(read, "%d %d\n", &n, &m);
	fprintf(write, "%d %d\n", 100, 10);
	int x, y;
	double distance = 0.0;

	//	while (feof(read)!=true )
	//	{
	for (size_t i=0; i < 500 - 1; i++)
	{
		for (int j = i + 1; j < 500; j++)
		{

			fscanf(read, "%d %d %lf\n", &x, &y, &distance);
			if (x<100 && y<100)
			fprintf(write, "%d %d %.2f\n", x, y, distance);
		}
	}
	fclose(read);
	fclose(write);

}*/
void Maintenance::Formulations(int a)
{
	M_Mixed_Risk = false;
	M_Formulacion_Cuantiles = false;
	M_Max_Formulation = false;
	M_Min_Formulation = false;
	M_ComputationTime_Grasp = 1;

	if (a >= 3)
		M_Min_Formulation = true;

	if (a == 2 || a == 4)
	{
		M_ComputationTime_Grasp = 1;

		GRASP();
		cout << "PP" << endl;
		Set_Max_Riesgo(Get_Best_Obj() * Get_T());
	}
	if (a >= 1)
	{
		Set_Max_Formulation(true);
	}

	IntegerFormulation(0);


}
