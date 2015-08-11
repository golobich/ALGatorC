//
//  main.cpp
//  AlgatorC
//
//  Created by Tadej Golobič on 28/06/15.
//  Copyright © 2015 Tadej Golobič. All rights reserved.
//
#define LOGGING_LEVEL_1

#include <iostream>
#include <sys/stat.h>
#include <dirent.h>
#include <dlfcn.h>
#include <unistd.h>
#include <fstream>
#include <algorithm>
#include "AbsAlgorithm.hpp"
#include "ETestSet.hpp"
#include "TestSetIterator.hpp"
#include "Log.hpp"
#include <string>
#include <memory>
_INITIALIZE_EASYLOGGINGPP

#ifdef __cplusplus
extern "C" {
#endif
    AbsAlgorithm* (*create_alg)();
    void (*destroy_alg)(AbsAlgorithm*);

    TestSetIterator* (*create_it)();
    void (*destroy_it)(TestSetIterator*);
#ifdef __cplusplus
}
#endif

const char *ALGATOR_ROOT = "ALGATOR_ROOT";

bool write_to_atts(const std::string &f, const char *test_name)
{
    std::ofstream file(f);
    if (file.is_open())
    {
        file << "{\"TestSet\" : {" << std::endl;
        file << "\t\"ShortName\" : \"...\"," << std::endl;
        file << "\t\"Description\" : \"...\"," << std::endl;
        file << "\t\"HTMLDescFile\" : \"...\"," << std::endl;
        file << "\t\"N\" : 1," << std::endl;
        file << "\t\"TestRepeat\" : 1," << std::endl;
        file << "\t\"QuickTest\" : true," << std::endl;
        file << "\t\"TestSetFiles\" : [...]," << std::endl;
        file << "\t\"DescriptionFile\" : \"" << test_name << ".txt" << "\"," << std::endl;
        file << "\t}" << std::endl;
        file << "}";
        file.close();
        return true;
    }
    return false;
}

std::map<std::string, std::vector<std::wstring>> get_result_parameters(const std::string &f_name)
{
	std::string content = "";
	std::ifstream f(f_name);
	if (f.is_open())
	{
	    std::string line = "";
	    while (!f.eof())
	    {
	    	getline(f, line);
	        content += line;
	    }
	    f.close();
	}


    std::map<std::string, std::vector<std::wstring>> res;
    std::string delimiter;
    JSONValue *value = JSON::Parse(content.c_str());
    if (value == NULL)
    {
        return res;
    }
    if (value->IsObject())
    {
        JSONObject object = value->AsObject();
        std::wstring ws = L"ResultDescription";
        if (object.find(ws) != object.end())
        {
            if (object[ws]->IsObject())
            {
                object = object[ws]->AsObject();
                ws = L"Format";
                if (object.find(ws) != object.end())
                {
                    if (object[ws]->IsString())
                    {
                        std::wstring format (object[ws]->AsString());
                        std::vector<std::wstring> tmp_v;
                        tmp_v.push_back(format);
                        res["Format"] = tmp_v;
                        std::string tmp (format.begin(), format.end());
                        if (tmp != "CSV")
                        {
                            LOG(ERROR) << "Format must be CSV!";
                            return res;
                        }
                    }else
                    {
                        LOG(ERROR) << "Format must be string type";
                        return res;
                    }
                }else
                {
                    LOG(ERROR) << "Can't find Format field";
                }
                ws = L"Delimiter";
                if (object.find(ws) != object.end())
                {
                    if (object[ws]->IsString())
                    {
                        std::wstring del (object[ws]->AsString());
                        std::string tmp (del.begin(), del.end());
                        std::vector<std::wstring> tmp_v;
                        tmp_v.push_back(del);
                        res["Delimiter"] = tmp_v;
                        delimiter = tmp;
                    }else
                    {
                        LOG(ERROR) << "Delimiter must be string type";
                        return res;
                    }
                }else
                {
                    LOG(ERROR) << "Can't find Delimiter field";
                    return res;
                }
                ws = L"TestParameters";
                if (object.find(ws) != object.end())
                {
                    if (object[ws]->IsArray())
                    {
                        JSONArray arr = object[ws]->AsArray();
                        std::vector<std::wstring> tmp_v;
                        for (unsigned int i = 0; i<arr.size(); i++)
                        {
                            tmp_v.push_back(arr[i]->Stringify());
                        }
                        res["TestParameters"] = tmp_v;
                    }else
                    {
                        LOG(ERROR) << "TestParameters field must be array";
                        return res;
                    }
                }else
                {
                    LOG(ERROR) << "Can't find TestParameters field";
                    return res;
                }
                ws = L"ResultParameters";
                if (object.find(ws) != object.end())
                {
                    if (object[ws]->IsArray())
                    {
                        JSONArray arr = object[ws]->AsArray();
                        std::vector<std::wstring> tmp_v;
                        for (unsigned int i = 0; i<arr.size(); i++)
                        {
                            tmp_v.push_back(arr[i]->Stringify());
                        }
                        res["ResultParameters"] = tmp_v;
                    }else
                    {
                        LOG(ERROR) << "Field ResultParameters must be array";
                        return res;
                    }
                }else
                {
                    LOG(ERROR) << "Can't find ResultParameters field";
                }
            }else
            {
                LOG(ERROR) << "ResultDescription field must be JSONObject";
                return res;
            }
        }else
        {
            LOG(ERROR) << "Can't find ResultDescription field!";
            return res;
        }
    }
    return res;
}

std::string get_project_dir(std::string proj_name)
{
	std::string s = getenv(ALGATOR_ROOT);
	s.append("/data_root/projects/PROJ-");
	s.append(proj_name);
	s.append("/");
	return s;
}

bool execute_system_command(const char *cmd)
{
    LOG(INFO) << "Executing cmd: \"" << cmd << "\"";
    const int err = system(cmd);
    if (err == -1)
    {
        LOG(ERROR) << "Error occured while trying to execute command " << cmd;
        return false;
    }
    return true;
}

bool build_lib(const std::string &lib_name, const std::string &source)
{
    struct tm *lib_clock, *src_clock;
    struct stat lib_attrib, src_attrib;

    stat(lib_name.c_str(), &lib_attrib);
    lib_clock = gmtime(&(lib_attrib.st_mtime));
    time_t lib_time = mktime(lib_clock);

    stat(source.c_str(), &src_attrib);
    src_clock = gmtime(&(src_attrib.st_mtime));
    time_t src_time = mktime(src_clock);

    if (difftime(lib_time, src_time) < 0)
        return true;
    return false;
}

bool build_project_shared_library(const char *proj_name, const char *alg_name)
{
    std::string proj_dir = get_project_dir(proj_name);
    //g++ -std=gnu++11 -fPIC ../proj/src/* ../algs/ALG-BubbleSort/src/BubbleSortAlgorithm.cpp -shared -o libProject.so -Wl,--whole-archive -lAlgatorc -Wl,--no-whole-archive
    std::string ld = "g++ -std=gnu++11 -fPIC "; ld.append(proj_dir); ld.append("algs/ALG-");
    ld.append(alg_name); ld.append("/src/"); ld.append(alg_name); ld.append("Algorithm.cpp ");
    ld.append(proj_dir); ld.append("proj/src/"); ld.append(proj_name); ld.append("AbsAlgorithm.cpp ");
    ld.append(proj_dir); ld.append("proj/src/"); ld.append(proj_name); ld.append("TestCase.cpp ");
    ld.append(proj_dir); ld.append("proj/src/"); ld.append(proj_name); ld.append("TestSetIterator.cpp ");
    ld.append("-shared -o "); ld.append(proj_dir); ld.append("lib/"); ld.append("libProject.so -Wl,--whole-archive -lAlgatorc -Wl,--no-whole-archive");

    return execute_system_command(ld.c_str());
}

void print_dir(const std::string &dir)
{
    struct stat sb;
    if (stat(dir.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
    {
        DIR *pDIR;
        struct dirent *entry;
        if( (pDIR=opendir(dir.c_str())) )
        {
            while( (entry = readdir(pDIR)) )
            {
                if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 )
                {
                    std::cout << entry->d_name << std::endl;
                }
            }
            closedir(pDIR);
        }
    }
}

bool contains(std::vector<std::string> &set, std::string key)
{
    for (unsigned int i = 0; i<set.size(); i++)
    {
        std::string tmp = set.at(i);
        std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
        //std::cout << "Searching for " << key << " in " << tmp << std::endl;
        if (tmp == key)
        {
            //std::cout << "Returning true" << std::endl;
            return true;
        }
    }
    //std::cout << "Returning false" << std::endl;
    return false;
}

void write_result(const char* proj_name, const std::string &alg_name, const std::string test_set, std::map<std::string, std::vector<std::wstring>> params, std::chrono::duration<double> *times, int size, int test_num, bool done, TestCase *t_case)
{
    LOG(INFO) << "Writing to results for test #" << test_num;

	std::string file = get_project_dir(proj_name); file.append("results/"); file.append(alg_name);
	file.append("-"); file.append(test_set); file.append(".em");
	std::ofstream f(file, std::ios_base::app);
	std::vector<std::string> parameters;
	parameters.push_back(alg_name);
	parameters.push_back(test_set);
	parameters.push_back("Done");
    std::vector<std::wstring> tmp_v1 = params["Format"];
    std::wstring ws_f = tmp_v1.at(0);
    tmp_v1 = params["Delimiter"];
    std::wstring tmp_s = tmp_v1.at(0);
    ParameterSet *parameters_set = t_case->get_parameters();
    
    std::string format = "CSV";
    std::string delimiter = ";";
    //std::string format(ws_f.begin(), ws_f.end());
    //std::string delimiter(tmp_s.begin(), tmp_s.end());
    parameters.push_back(parameters_set->print_for_results(format, delimiter[0]));

    
    for (std::map<std::string, std::vector<std::wstring>>::iterator it = params.begin(); it != params.end(); ++it)
    {
        if (it->first == "TestParameters" || it->first == "ResultParameters")
        {
            std::vector<std::wstring> tmp_v = params[it->first];
        	for (unsigned int i = 0; i<tmp_v.size(); i++)
        	{
                std::wstring s = tmp_v.at(i);
                std::string tmp (s.begin()+1, s.end()-1);
                parameters.push_back(tmp);
        	}
        }
    }

    bool t_max = contains(parameters, "tmax");
    bool t_min = contains(parameters, "tmin");
    bool t_avg = contains(parameters, "tavg");
    bool t_sum = contains(parameters, "tsum");
    bool check = contains(parameters, "check");
    std::chrono::duration<double> min, max, avg, sum;
    min = times[0]; max = times[0]; sum = times[0];
    for (int i = 1; i<size; i++)
    {
        if (times[i] < min)
        {
            min = times[i];
        }
        if (times[i] > max)
        {
            max = times[i];
        }
        sum += times[i];
    }
    avg = sum / size;
	if (f.is_open())
	{
        f << parameters.at(0) << delimiter;
        f << parameters.at(1) << delimiter;
        f << parameters.at(2) << delimiter;
        f << parameters.at(3);
        if (t_min)
        {
            f << min.count() * 1000 << delimiter;
        }
        if (t_max)
        {
            f << max.count() * 1000 << delimiter;
        }
        if (t_avg)
        {
            f << avg.count() * 1000 << delimiter;
        }
        if (t_sum)
        {
            f << sum.count() * 1000 << delimiter;
        }
        if (check)
        {
            if (done)
            {
                f << "OK" << delimiter;
            }else
            {
                f << "NOK" << delimiter;
            }
        }
        f << std::endl;
	}else
	{
		LOG(ERROR) << "Can't write to file " << file;
		return;
	}

}

void execute_algatorc(const char* proj, const std::string &alg, const std::string test_set)
{
    std::string proj_dir = get_project_dir(proj);
    std::string test_set_path = proj_dir; test_set_path.append("tests/");
    test_set_path.append(test_set); test_set_path.append(".atts");

    ETestSet tests = ETestSet(test_set_path);
    std::string test_set_file = proj_dir; test_set_file.append("tests/"); test_set_file.append(tests.get_test_set_description_file());

    LOG(INFO) << "Executing algatorc with parameters: " << proj << ", " << alg << ", " << test_set;
    std::string project_lib = proj_dir; project_lib.append("lib/libProject.so");
    sleep(2);
    void *handle = dlopen(project_lib.c_str(), RTLD_LAZY);
    if (handle)
    {
        create_alg = (AbsAlgorithm* (*)())dlsym(handle, "create_algorithm_object");
        destroy_alg = (void(*)(AbsAlgorithm*))dlsym(handle, "destroy_algorithm_object");
    	create_it = (TestSetIterator* (*)())dlsym(handle, "create_iterator_object");
    	destroy_it = (void(*)(TestSetIterator*))dlsym(handle, "destroy_iterator_object");
    	AbsAlgorithm *a = (AbsAlgorithm*)create_alg();
    	TestSetIterator *it = (TestSetIterator*)create_it();

    	char *sdup = strdup(proj);
        it->set_project_name(sdup);
        it->set_test_set(tests);
        ETestSet *t = it->get_test_set();
        int repeat = std::atoi(t->get(ETestSet::id_test_repeat).c_str());

        std::string s = "mkdir -p "; s.append(get_project_dir(proj)); s.append("results");
        if (!execute_system_command(s.c_str()))
        {
        	LOG(ERROR) << "Can't create result directory! Exiting ";
        	return;
        }

        std::string file = get_project_dir(proj); file.append("proj/"); file.append(proj);
        file.append("-em.atrd");

        std::map<std::string, std::vector<std::wstring>> v = get_result_parameters(file);
        TestCase *t_case;
        int cnt = 0;
        while(it->has_next())
        {
        	cnt++;
            it->read_next();
            std::chrono::duration<double> alg_time[repeat];
            bool alg_ok = true;
            bool executed = false;
            for (int i = 0; i<repeat; i++)
            {
            	t_case = it->get_current();
            	if (t_case != nullptr)
            	{
					bool ok = a->init(t_case);
					if (ok)
					{
                        executed = true;
						a->timer.start();
						a->run();
						a->timer.stop();
						alg_time[i] = a->timer.time();
                        if (!a->done())
                        {
                            alg_ok = false;
                            break;
                        }
					}else
					{
						LOG(ERROR) << "Error occured while trying to init TestCase";
						return;
					}
            	}
            }
            if (executed)
            {
                write_result(proj, alg, test_set, v, alg_time, repeat, cnt, alg_ok, t_case);
            }
        }
        destroy_alg(a);
        destroy_it(it);
    }
    dlclose(handle);
}

//! Create new algatorc project
//! \param const char *proj_name name of the project
//! \return bool return true if project is successfuly created
bool create_project(const char *proj_name)
{
    //PROJ-argv[2]/{libs, tests, proj/{src, include}}
	std::string s = "mkdir -p "; s.append(get_project_dir(proj_name));
	s.append("tests");
    LOG(INFO) << "Creating new project " << proj_name;
    if (!execute_system_command(s.c_str()))
    {
        return false;
    }

    s = "mkdir -p "; s.append(get_project_dir(proj_name)); s.append("lib/");
    if (!execute_system_command(s.c_str()))
    {
        return false;
    }

    s = "mkdir -p "; s.append(get_project_dir(proj_name)); s.append("proj/src");
    if (!execute_system_command(s.c_str()))
    {
        return false;
    }

    s = "touch "; s.append(get_project_dir(proj_name)); s.append("tests/testset1.txt");
    if (!execute_system_command(s.c_str()))
    {
        return false;
    }

    s = get_project_dir(proj_name); s.append("tests/TestSet1.atts");
    if (!write_to_atts(s, "TestSet1"))
    {
        return false;
    }

    //create source files content
    s = get_project_dir(proj_name);
    s.append("proj/src/"); s.append(proj_name); s.append("TestCase.cpp");
    std::ofstream test_case_file(s);
    if (test_case_file.is_open())
    {
        test_case_file << "#include<iostream>" << std::endl;
        test_case_file << "#include<TestCase.hpp>" << std::endl << std::endl;
        s = "class "; s.append(proj_name); s.append("TestCase : public TestCase {");
        test_case_file << s << std::endl;
        test_case_file << "public: " << std::endl;
        test_case_file << "\tstd::string print() { " << std::endl;
        test_case_file << "\t\t //TODO: implement method" << std::endl;
        test_case_file << "\t}" << std::endl;
        test_case_file << "};";
        test_case_file.close();
    }else
    {
        LOG(ERROR) << "Can't write content to file " << s;
        return false;
    }

    s = get_project_dir(proj_name);
    s.append("proj/"); s.append(proj_name); s.append("-em.atrd");
    std::ofstream p_file(s);
    if (p_file.is_open())
    {
    	p_file << "{" << std::endl;
    	p_file << "\t\"ResultDescription\" : \n\t{" << std::endl;
    	p_file << "\t\t\"Format\" : \"CSV\", " << std::endl;
    	p_file << "\t\t\"Delimiter\" : \";\"," << std::endl;
    	p_file << "\t\t\"TestParameters\" : [ ... ]," << std::endl;
    	p_file << "\t\t\"ResultParameters\" : [ ... ], " << std::endl << std::endl;
    	p_file << "\t\t\"Parameters\" : [ ... ]" << std::endl;
    	p_file << "\t}" << std::endl;
    	p_file << "}" << std::endl;
    	p_file.close();
    }else
    {
        LOG(ERROR) << "Can't write content to file " << s;
        return false;
    }

    s = get_project_dir(proj_name);
    s.append("proj/src/"); s.append(proj_name); s.append("TestSetIterator.cpp");
    std::ofstream test_set_file(s);
    if (test_set_file.is_open())
    {
        test_set_file << "#include <iostream>" << std::endl;
        test_set_file << "#include <TestSetIterator.hpp>" << std::endl << std::endl;
        s = "class "; s.append(proj_name); s.append("TestSetIterator : public TestSetIterator {");
        test_set_file << s << std::endl;
        test_set_file << "public: " << std::endl;
        test_set_file << "\tTestCase *get_current(){" << std::endl;
        test_set_file << "\t\t //TODO: implement method" << std::endl;
        test_set_file << "\t\t return nullptr; \n\t}\n};" << std::endl << std::endl;
        test_set_file << "#ifdef __cplusplus" << std::endl << "extern \"C\" {" << std::endl << "#endif" << std::endl;
        test_set_file << "TestSetIterator * create_iterator_object() {" << std::endl;
        s = "\treturn new "; s.append(proj_name); s.append("TestSetIterator;");
        test_set_file << s << std::endl << "}" << std::endl << std::endl;
        test_set_file << "void destroy_iterator_object(TestSetIterator* object) {" << std::endl;
        test_set_file << "\tdelete object;" << std::endl;
        test_set_file << "}" << std::endl;
        test_set_file << "#ifdef __cplusplus" << std::endl << "}" << std::endl << "#endif";
        test_set_file.close();
    }else
    {
        LOG(ERROR) << "Can't write content to file " << s;
        return false;
    }

    s = get_project_dir(proj_name); s.append("proj/src/"); s.append(proj_name);
    s.append("AbsAlgorithm.cpp");
    std::ofstream abs_algorithm_file(s);
    if (abs_algorithm_file.is_open())
    {
        abs_algorithm_file << "#include <iostream>" << std::endl;
        abs_algorithm_file << "#include <AbsAlgorithm.hpp>" << std::endl;
        s = "include \""; s.append(proj_name); s.append("TestCase.cpp\"");
        abs_algorithm_file << s << std::endl;
        s = "class "; s.append(proj_name); s.append("AbsAlgorithm : public AbsAlgorithm {");
        abs_algorithm_file << s << std::endl;
        std::string test_c = proj_name; test_c.append("_test_case;");
        std::transform(test_c.begin(), test_c.end(), test_c.begin(), ::tolower);
        s = "\t"; s.append(proj_name); s.append("TestCase *"); s.append(test_c);
        abs_algorithm_file << "private: " << std::endl;
        abs_algorithm_file << s << std::endl;
        abs_algorithm_file << "public:" << std::endl;
        abs_algorithm_file << "\tbool init (TestCase *test) {\n\t\t //TODO: implement method\n\t\t return true; \n\t}" << std::endl << std::endl;
        abs_algorithm_file << "\tvoid run() {\n\t\t //TODO: Implement method \n\t}" << std::endl << std::endl;
        abs_algorithm_file << "\tvoid done() {\n\t\t //TODO: Implement method \n\t}" << std::endl;
        abs_algorithm_file << "};";
        abs_algorithm_file.close();
    }else
    {
        LOG(ERROR) << "Can't write content to file " << s;
        return false;
    }

    return true;
}

//! Create new algorithm in project
//! \param const char *proj_name name of the project
//! \param const char *alg_name name of algorithm you want to create
//! \return bool return true if algorithm is successfuly created
bool create_algorithm(const char *proj_name, const char *alg_name)
{
    struct stat sb;
    std::string buff = get_project_dir(proj_name);
    if (stat(buff.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
    {
        //PROJ-proj_name/algs/alg_name/src
        LOG(INFO) << "Creating algorithm " << alg_name << " in project " << proj_name;
        buff = "mkdir -p "; buff.append(get_project_dir(proj_name)); buff.append("algs/ALG-"); buff.append(alg_name); buff.append("/src/");
        if (!execute_system_command(buff.c_str()))
        {
            return false;
        }

        //fill content of files
        buff = get_project_dir(proj_name);
        buff.append("algs/ALG-"); buff.append(alg_name); buff.append("/src/"); buff.append(alg_name);
        buff.append("Algorithm.cpp");
        
        std::ofstream file(buff.c_str());
        if (file.is_open())
        {
            file << "#include <iostream>" << std::endl;
            buff = "../../../proj/src/"; buff.append(proj_name); buff.append("AbsAlgorithm.cpp");
            file << "#include \"" << buff << "\"" << std::endl << std::endl;
            buff = "class "; buff.append(alg_name); buff.append("Algorithm : public "); buff.append(proj_name);
            buff.append("AbsAlgorithm {");
            file << buff << std::endl;
            file << "public:" << std::endl;
            file << "\tvoid execute() {";
            file << "\t\t //Implement you algorithm here" << std::endl;
            file << "\t}" << std::endl;
            file << "};" << std::endl << std::endl;
            file << "#ifdef __cplusplus" << std::endl << "extern \"C\" {" << std::endl << "#endif" << std::endl;
            file << "AbsAlgorithm * create_algorithm_object() {" << std::endl;
            buff = "\treturn new "; buff.append(alg_name); buff.append("Algorithm;");
            file << buff << std::endl << "}" << std::endl << std::endl;
            file << "void destroy_algorithm_object(AbsAlgorithm* object) {" << std::endl;
            file << "\tdelete object;" << std::endl;
            file << "}" << std::endl;
            file << "#ifdef __cplusplus" << std::endl << "}" << std::endl << "#endif";
            file.close();

        }else
        {
            LOG(ERROR) << "Can't create content of " << buff;
            return false;
        }

        //create JSON content
        buff = get_project_dir(proj_name);
        buff.append("algs/ALG-"); buff.append(alg_name); buff.append("/"); buff.append(alg_name);
        buff.append(".atal");
        std::ofstream json_file(buff.c_str());
        if (json_file.is_open())
        {
            json_file << "{" << std::endl;
            json_file << "\tAlgorithm : {" << std::endl;
            char temp[100];
            sprintf(temp, "\t\tShortName\t\t : \"%s\",", alg_name);
            json_file << temp << std::endl;
            json_file << "\t\tDescription\t\t : \"\"," << std::endl;
            json_file << "\t\tHTMLDescFile\t\t : \"\"," << std::endl;
            json_file << "\t\tAuthor\t\t : \"\"," << std::endl;
            json_file << "\t\tDate\t\t : \"\"" << std::endl;
            json_file << "\t}" << std::endl;
            json_file << "}" << std::endl;
            json_file.close();
        }else
        {
            LOG(ERROR) << "Can't create content of " << buff;
            return false;
        }

        LOG(INFO) << "Algorithm " << alg_name << " in project " << proj_name << " is succesfuly created";
        return true;
    }else
    {
        LOG(ERROR) << "Project " << proj_name << "doesn't exist";
        return false;
    }
    return false;
}

bool dir_exist(const char *dir)
{
    struct stat sb;
    return stat(dir, &sb) == 0 && S_ISDIR(sb.st_mode);
}

bool create_test_set(const char *proj_name, const char *test_name)
{
    std::string proj_dir = get_project_dir(proj_name);
    if (dir_exist(proj_dir.c_str()))
    {
        std::string cmd;
        std::string test_dir = proj_dir; test_dir.append("tests/");
        if (!dir_exist(test_dir.c_str()))
        {
            cmd = "mkdir -p "; cmd.append(test_dir); 
            if (!execute_system_command(cmd.c_str()))
            {
                LOG(ERROR) << "Can't create tests folder";
                return false;
            }
        }
        
        cmd = "touch "; cmd.append(test_dir); cmd.append(test_name); cmd.append(".txt");
        if (!execute_system_command(cmd.c_str()))
        {
            return false;
        }
        std::string f = test_dir; f.append(test_name); f.append(".atts");
        if (!write_to_atts(f, test_name))
        {
            LOG(ERROR) << "Can't write to atts file " << f;
            return false;
        }

        std::cout << "TestSet " << test_name << " was successfuly created in project " << proj_name << std::endl;
        return true;
    }else
    {
        std::cout << "Project " << proj_dir << " doesn't exist" << std::endl;
        return false;
    }
    return false;
}

bool show_help()
{
    return execute_system_command("man algatorc");
}


//#include <typeinfo>
//argv[0] == executable name
//argv[1] == option
//argv[2] == PROJECT name

//--execute
//argv[3] == ALGORITHM name (BubbleSort)
//argv[4] == TestCase file (TestCase1)

//--add_algorithm
//argv[3] == ALGORITHM_name (BubbleSort)

//--add_test
//argv[3] == TESTSET_name (TestSet1)
int main(int argc, const char *argv[])
{

	easyloggingpp::Configurations defaultConf;
	defaultConf.setToDefault();

    if (argc == 3)
    {
        if (strcmp(argv[1], "-n") == 0 || strcmp(argv[1], "--new_project") == 0)
        {
                if (!create_project(argv[2]))
                {
                    return 8;
                }
        }else
        {
            LOG(ERROR) << "Check -h or --help for usage of algatorc";
            return 7;
        }
    }
    else if (argc == 2)
    {
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
        {
            if (!show_help())
            {
                return 36;
            }
        }
        else if (strcmp(argv[1], "-v") == 0|| strcmp(argv[1], "--version") == 0)
        {
            std::cout << "algatorc 1.0" << std::endl;
        }
        else
        {
            LOG(ERROR) << "Check -h or --help for usage of algatorc";
        }
    }
    else if (argc > 3)
    {
        if (strcmp(argv[1], "-a") == 0 || strcmp(argv[1], "--add_algorithm") == 0)
        {
            for (int i = 3; i<argc; i++)
            {
                if (!create_algorithm(argv[2], argv[i]))
                {
                    return 8;
                }
            }
        }
        else if (strcmp(argv[1], "-t") == 0 || strcmp(argv[1], "--add_test") == 0)
        {
            for (int i = 3; i<argc; i++)
            {
                if (!create_test_set(argv[2], argv[i]))
                {
                    return 42; 
                }
            }
        }
        else if ((strcmp(argv[1], "-e") == 0 || strcmp(argv[1], "--execute") == 0) && argc == 5)
        {
            LOG(INFO) << "===== START =====";
            struct stat sb;

            std::string alg_env = getenv(ALGATOR_ROOT);
            if (alg_env.empty())
            {
            	return 24;
            }

            std::string lib_dir (alg_env);
            lib_dir.append("/data_root/projects/PROJ-"); lib_dir.append(argv[2]); lib_dir.append("/lib/");
            if (stat(lib_dir.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
            {
                DIR *pDIR;
                struct dirent *entry;
                if( (pDIR=opendir(lib_dir.c_str())) )
                {
                    bool build_library = true;
                    while( (entry = readdir(pDIR)) )
                    {
                        std::string lib_name = "libProject.so";
                        if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 )
                        {
                            if (strcmp(entry->d_name, lib_name.c_str()) == 0)
                            {
                                std::string lib_path = lib_dir + lib_name;

                                std::string proj_dir = getenv(ALGATOR_ROOT); proj_dir.append("/data_root/projects/PROJ-");
                                proj_dir.append(argv[2]); proj_dir.append("/");

                                std::string test_case_src = proj_dir; test_case_src.append("/proj/src/");
                                test_case_src.append(argv[2]); test_case_src.append("TestCase.cpp");

                                std::string test_set_src = proj_dir; test_set_src.append("/proj/src/");
                                test_set_src.append(argv[2]); test_set_src.append("TestSetIterator.cpp");

                                std::string abs_alg_src = proj_dir; abs_alg_src.append("/proj/src/");
                                abs_alg_src.append(argv[2]); abs_alg_src.append("AbsAlgorithm.cpp");

                                std::string alg_src = proj_dir; alg_src.append("/algs/ALG-"); alg_src.append(argv[3]);
                                alg_src.append("/src/"); alg_src.append(argv[3]); alg_src.append("Algorithm.cpp");

                                build_library = !(!build_lib(lib_path, test_case_src) && !build_lib(lib_path, test_set_src) && !build_lib(lib_path, abs_alg_src) && !build_lib(lib_path, alg_src));
                            }
                        }
                    }
                    closedir(pDIR);

                    if (build_library)
                    {
                        LOG(INFO) << "Need to build project library";
                        if (!build_project_shared_library(argv[2], argv[3]))
                        {
                            return 10;
                        }
                        LOG(INFO) << "Library was successfuly build";
                    }

                    execute_algatorc(argv[2], argv[3], argv[4]);
                }
            }else
            {
                LOG(INFO) << "Creating lib folder";

                std::string buff = "mkdir -p "; buff.append(getenv(ALGATOR_ROOT));
                buff.append("data_root/projects/PROJ-"); buff.append(argv[2]);
                buff.append("/lib/");

                if (execute_system_command(buff.c_str()))
                {
                	if (build_project_shared_library(argv[2], argv[3]))
                	{
                		execute_algatorc(argv[2], argv[3], argv[4]);
                	}
                }
            }
        }
        else
        {
            LOG(ERROR) << "Check -h or --help for usage of algatorc";
            return 9;
        }
    }
    else
    {
        LOG(ERROR) << "Wrong usage of algatorc. Try typing algatorc -h or algatorc --help";
        return 9;
    }

	return 0;
}
