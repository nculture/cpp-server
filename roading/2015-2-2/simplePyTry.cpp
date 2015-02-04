#include <iostream>
#include "Python.h"
#include "windows.h"

PyObject** funcs;

bool initFuncs()
{
  PyObject* pName = PyString_FromString("pyexample");
  PyObject* pModule = PyImport_Import(pName);
  if(!pModule)
  {
    std::cout << "can't find pyexample.\n";
    return false;
  }

  PyObject* pDict = PyModule_GetDict(pModule);
  if(!pDict)
  {
    std::cout << "pModule fail.\n";
    return false;
  }

  PyObject* pFunc = PyDict_GetItemString(pDict, "AddMult");
  if(!pFunc || !PyCallable_Check(pFunc))
  {
    std::cout << "can't find function [AddMult]\n";
    return false;
  }
  funcs[0] = pFunc;

  pFunc = PyDict_GetItemString(pDict, "Minus");
  if(!pFunc || !PyCallable_Check(pFunc))
  {
    std::cout << "can't find function [Minus]\n";
    return false;
  }
  funcs[1] = pFunc;

  //Py_DECREF(pDict);
  //Py_DECREF(pModule);
  //Py_DECREF(pName);

  return true;
}

void basicPy()
{
    PyObject* pArgs = Py_BuildValue("ii", 12, 14);

    PyObject* pRet = PyEval_CallObject(funcs[0], pArgs);

    int a = 0;

    int b = 0;

    if (pRet && PyArg_ParseTuple(pRet,"ii", &a,&b))

    {

           printf("Function[AddMult] call successful a + b = %d, a * b = %d\n", a, b);

    }

    pArgs = Py_BuildValue("ii", 20, 12);

    pRet = PyEval_CallObject(funcs[1], pArgs);

    a = 0;

    b = 0;

    if (pRet && PyArg_Parse(pRet,"i", &a))

    {

           printf("Function[Minus] call successful a - b = %d\n", a);

    }
    else 
    {
      std::cout << "minus failed.\n";
    }
}

int main()
{
  Py_Initialize();
  if(!Py_IsInitialized())
  {
    return -1;
  }

  PyRun_SimpleString("import os");
  PyRun_SimpleString("import gc");
  PyRun_SimpleString("import objgraph");
  PyRun_SimpleString("gc.collect()");
  PyRun_SimpleString("objgraph.show_growth()");

  funcs = new PyObject*[2];

  if(!initFuncs())
  {
    std::cout << "initFuncs fail.\n";
    return -1;
  }

  Sleep(10000);

	basicPy();

  Sleep(10000);

  for(int i = 0; i < 2; i++)
  {
    if(funcs[i])
    {
      //Py_DECREF(funcs[i]);
    }
  }

  Py_Finalize();
	return 0;
}