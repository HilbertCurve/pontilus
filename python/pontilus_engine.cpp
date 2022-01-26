#define PY_SSIZE_T_CLEAN
#include <Python.h>

#define PONT_PY_FUNC_DECL static PyObject *

// audio
#include "audio/AudioMaster.h"
PONT_PY_FUNC_DECL
p_initAudio(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ""))
        return NULL;
    Pontilus::Audio::initAudio();

    Py_RETURN_NONE;
}

static PyMethodDef Pont_Methods[] = {
    {"init_audio", p_initAudio, METH_NOARGS, 
    "Initialize Pontilus audio system. This shouldn't exist; it's handled Pontilus::init()"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef Pont_Module {
    PyModuleDef_HEAD_INIT,
    "pontilus",
    NULL,
    -1,
    Pont_Methods
};

PyMODINIT_FUNC
PyInit_Pontilus()
{
    return PyModule_Create(&Pont_Module);
}
