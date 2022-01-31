#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"

#define PONT_PY_FUNC_DECL static PyObject *
#define PONT_PY_TYPE_NEW static PyObject *
#define PONT_PY_TYPE_INIT static int
#define PONT_PY_TYPE_DEALLOC static void

extern "C++"
{

// audio

// Functions and types used here:
// Audio::WAVFile
// Audio::initWAVFile()
// Audio::clearWAVFile()

#include "audio/AudioMaster.h"
using namespace Pontilus;

typedef struct {
    PyObject_HEAD
    const char *p_filepath_cp;
    Audio::WAVFile *val;
} p_WAVFile;

PONT_PY_TYPE_NEW
p_WAVFile_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    p_WAVFile *self;
    self = (p_WAVFile *) type->tp_alloc(type, 0);
    if (self != NULL)
    {
        self->val = (Audio::WAVFile *) malloc(sizeof(Audio::WAVFile));
    }
    return (PyObject *) self;
}

PONT_PY_TYPE_INIT
p_WAVFile_init(p_WAVFile *self, PyObject *args, PyObject *kwds)
{
    static char *kwlist[] = { "filepath", NULL };
    const char *filepath;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O&", kwlist, 
                                     PyUnicode_FSConverter, filepath)) 
        return -1;
    
    if (!Audio::initWAVFile(*self->val, filepath))
    {
        // i don't know if python deallocates upon returning -1; i'll assume so
        return -1; // TODO: better error handling
    }
    self->p_filepath_cp = filepath;

    return 0;
}

PONT_PY_TYPE_DEALLOC
p_WAVFile_dealloc(p_WAVFile *self)
{
    Audio::clearWAVFile(*self->val);
    free(self->val);
    Py_TYPE(self)->tp_free((PyObject *) self);
}

static PyMemberDef p_WAVFile_def[] = {
    { "filepath", T_STRING, offsetof(p_WAVFile, p_filepath_cp), 0, 
    "Relative file path to the .wav file being streamed from."},
    {NULL}
};

static PyTypeObject p_WAVFile_t = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "pontilus.WAVFile",
    .tp_basicsize = sizeof(p_WAVFile),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor) p_WAVFile_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_doc = "Wrapper over .wav file.",
    .tp_init = (initproc) p_WAVFile_init,
    .tp_new = p_WAVFile_new
};

// core
// Functions and types used here:
// Engine::Scene
// init()

#include "core/Application.h"
#include "core/InputListener.h"
#include "core/Scene.h"

typedef struct {
    PyObject_HEAD
    PyObject *_init;
    PyObject *_update;
    PyObject *_clean;
    Engine::Scene *val;
} p_Scene;

PONT_PY_TYPE_NEW
p_Scene_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    p_Scene *self;
    self = (p_Scene *) type->tp_alloc(type, 0);

    if (self != NULL)
    {
        self->val = (Engine::Scene *) malloc(sizeof(Engine::Scene));
    }
    return (PyObject *) self;
}

PONT_PY_TYPE_INIT
p_Scene_init(p_Scene *self, PyObject *args, PyObject *kwds)
{
    static char *kwlist[] = { "init", "update", "clean", NULL };

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O&O&O&", kwlist/*...*/ )) 
        return -1;
    
    // TODO: function pointers don't play nicely with PyObject callbacks; find fix
}

PONT_PY_FUNC_DECL
p_Scene_set_init()
{

}

PONT_PY_FUNC_DECL
p_init(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ""))
    {
        return NULL;
    }
    Pontilus::init();

    Py_RETURN_NONE;
}



// module

static PyMethodDef Pont_Methods[] = {
    {"init", p_init, METH_VARARGS,
    "Initialize Pontilus engine. Call this function before using Pontilus and after pre-game initalization."}, 
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
PyInit_pontilus()
{
    PyObject *m;
    if (PyType_Ready(&p_WAVFile_t) < 0)
        return NULL;

    m = PyModule_Create(&Pont_Module);
    if (m == NULL)
        return NULL;
    
    Py_INCREF(&p_WAVFile_t);
    if (PyModule_AddObject(m, "WAVFile", (PyObject *) &p_WAVFile_t) < 0) {
        Py_DECREF(&p_WAVFile_t);
        Py_DECREF(m);
        return NULL;
    }

    return m;
}
}
