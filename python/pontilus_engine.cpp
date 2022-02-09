#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <structmember.h>
#include <string>

#define PONT_PY_FUNC_DECL static PyObject *
#define PONT_PY_TYPE_NEW static PyObject *
#define PONT_PY_TYPE_INIT static int
#define PONT_PY_TYPE_DEALLOC static void
#define PONT_PY_CONVERTER static int

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
    char *filepath;
    int len;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "s#", kwlist, 
                                     &filepath, &len)) 
        return -1;
    
    int a = Audio::initWAVFile(*self->val, filepath);
    if (a != 1)
    {
        std::string errstr;
        switch (a) {
            case -1:
                errstr = "No such file or directory: '" + std::string(filepath) + "'.";
                PyErr_SetString(PyExc_FileNotFoundError, errstr.c_str());
                break;
            case -2: 
                errstr = "File '" + std::string(filepath) + "' isn't a valid .wav file.";
                PyErr_SetString(PyExc_FileNotFoundError, errstr.c_str());
                break;
        }
        return -1;
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

static PyMemberDef p_WAVFile_member_def[] = {
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
    .tp_members = p_WAVFile_member_def,
    .tp_init = (initproc) p_WAVFile_init,
    .tp_new = p_WAVFile_new,
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

static p_Scene *_p_Scene_hoisted;

PONT_PY_CONVERTER
p_Scene_make_init(PyObject *pyfun, void (** _init)())
{
    if (!PyCallable_Check(pyfun)) return -1;

    _p_Scene_hoisted->_init = pyfun;
    Py_INCREF(pyfun);

    *_init = []() {
        static p_Scene *self = _p_Scene_hoisted;

        PyObject_CallObject(self->_init, NULL);
    };

    return 0;
}

PONT_PY_CONVERTER
p_Scene_make_update(PyObject *pyfun, void (** _update)(double dt))
{
    if (!PyCallable_Check(pyfun)) return -1;

    _p_Scene_hoisted->_update = pyfun;
    Py_INCREF(pyfun);

    *_update = [](double dt) {
        static p_Scene *self = _p_Scene_hoisted;

        PyObject_CallObject(self->_update, PyFloat_FromDouble(dt));
    };

    return 0;
}

PONT_PY_CONVERTER
p_Scene_make_clean(PyObject *pyfun, void (** _clean)())
{
    if (!PyCallable_Check(pyfun)) return -1;

    _p_Scene_hoisted->_clean = pyfun;
    Py_INCREF(pyfun);

    *_clean = []() {
        static p_Scene *self = _p_Scene_hoisted;

        PyObject_CallObject(self->_clean, NULL);
    };

    return 0;
}

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

    _p_Scene_hoisted = self;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|O&O&O&", kwlist, 
                                     p_Scene_make_init, _p_Scene_hoisted->val->init,
                                     p_Scene_make_update, _p_Scene_hoisted->val->update,
                                     p_Scene_make_clean, _p_Scene_hoisted->val->clean )) 
        return -1;
    
    return 0;
}

PONT_PY_FUNC_DECL
p_Scene_set_init(p_Scene *self, PyObject *args)
{
    _p_Scene_hoisted = self;

    if (!PyArg_ParseTuple(args, "O&", 
                          p_Scene_make_init, _p_Scene_hoisted->val->init)) 
        return NULL;

    return (PyObject *) self;
}

PONT_PY_TYPE_DEALLOC
p_Scene_dealloc(p_Scene *self)
{
    self->val->init = nullptr;
    self->val->update = nullptr;
    self->val->clean = nullptr;

    Py_DECREF(self->_init);
    Py_DECREF(self->_update);
    Py_DECREF(self->_clean);

    self->_init = nullptr;
    self->_update = nullptr;
    self->_clean = nullptr;

    free(self->val);
    Py_TYPE(self)->tp_free((PyObject *) self);
}

static PyMemberDef p_Scene_member_def[] = {
    { "objs", T_OBJECT,  }, // TODO: expose scene game objects field
    {NULL}
};

static PyMethodDef p_Scene_method_def[] = {
    { "set_init", (PyCFunction) p_Scene_set_init, METH_VARARGS, 
    "Set initialization callback of this scene. This callback will be run once this scene is switched to (or immediately if set as the first scene)." },
    {NULL}
};

static PyTypeObject p_Scene_t = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "pontilus.Scene",
    .tp_basicsize = sizeof(p_Scene),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor) p_Scene_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_doc = "An object representing game behavior when a game scene is initialized, updated, and destroyed.",
    .tp_methods = p_Scene_method_def,
    .tp_init = (initproc) p_Scene_init,
    .tp_new = p_Scene_new
};

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

static PyObject *p_CurrentScene = NULL;

PONT_PY_FUNC_DECL
p_set_scene(PyObject *dummy, PyObject *args)
{
    PyObject *result = NULL;
    PyObject *temp;

    if (!PyArg_ParseTuple(args, "O!:set_scene", &p_Scene_t, &temp))
    {
        return NULL;
    }

    Py_XINCREF(temp);
    Py_XDECREF(p_CurrentScene);
    p_CurrentScene = temp;

    Pontilus::setCurrentScene(*((p_Scene *) p_CurrentScene)->val);
    
    Py_INCREF(Py_None);
    result = Py_None;

    return result;
}

PONT_PY_FUNC_DECL
p_get_scene(PyObject *dummy, PyObject *args)
{
    if (!PyArg_ParseTuple(args, "")) return NULL;

    return p_CurrentScene;
}

// ecs
// 

#include "ecs/GameObject.h"
#include "ecs/Component.h"

typedef struct p_GameObject;

typedef struct {
    PyObject_HEAD
    p_GameObject *parent;
    Engine::ECS::Component *val;
} p_Component;

typedef struct {
    PyObject_HEAD
    PyObject *pos;
    PyObject *color;
    p_Component
    Engine::ECS::GameObject *val;
} p_GameObject;
// graphics


// module

static PyMethodDef Pont_Methods[] = {
    {"init", p_init, METH_VARARGS,
    "Initialize Pontilus engine. Call this function before using Pontilus and after pre-game initalization."}, 
    {"set_scene", p_set_scene, METH_VARARGS, 
    "Set the current running scene."},
    {"get_scene", p_get_scene, METH_NOARGS,
    "Obtain a reference to the current running scene."},
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
