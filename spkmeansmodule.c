#include <Python.h>
#include "matrix.h"

double **parseDataPoints(PyObject *data_points)
{
    int num_of_points = PyObject_Length(data_points);
    double **parsed_data_points = malloc(num_of_points * sizeof(double *));

    for (int i = 0; i < num_of_points; i++)
    {
        PyObject *row = PyList_GetItem(data_points, i);
        int num_of_cords = PyObject_Length(row);

        parsed_data_points[i] = malloc(num_of_cords * sizeof(double));
        for (int j = 0; j < num_of_cords; j++)
        {
            parsed_data_points[i][j] = PyFloat_AsDouble(PyList_GetItem(row, j));
        }
    }

    return parsed_data_points;
}

static PyObject *wrapWithPyObject(double **data_points, int rows, int columns)
{
    PyObject *lst = PyList_New(rows);

    for (int i = 0; i < rows; i++)
    {
        PyObject *inner_list = PyList_New(columns);
        for (int j = 0; j < columns; j++)
        {
            PyList_SetItem(inner_list, j, Py_BuildValue("d", data_points[i][j]));
        }

        PyList_SetItem(lst, i, inner_list);
    }

    return lst;
}

static PyObject *wam(PyObject *self, PyObject *args)
{
    PyObject *data_points;
    double **parsed_data_points;

    if (!PyArg_ParseTuple(args, "O", &data_points))
    {
        return NULL;
    }

    parsed_data_points = parseDataPoints(data_points);
    int num_of_points = PyObject_Length(data_points);
    int num_of_cords = PyObject_Length(PyList_GetItem(data_points, 0));

    return wrapWithPyObject(createWeightedAdjMatrix(parsed_data_points, num_of_points, num_of_cords),
                            num_of_points,
                            num_of_points);
}

static PyObject *ddg(PyObject *self, PyObject *args)
{
    PyObject *data_points;
    double **parsed_data_points;

    if (!PyArg_ParseTuple(args, "O", &data_points))
    {
        return NULL;
    }

    parsed_data_points = parseDataPoints(data_points);
    int num_of_points = PyObject_Length(data_points);
    int num_of_cords = PyObject_Length(PyList_GetItem(data_points, 0));

    return wrapWithPyObject(createDegMatrix(
                                createWeightedAdjMatrix(parsed_data_points, num_of_points, num_of_cords), num_of_points),
                            num_of_points,
                            num_of_points);
}

static PyMethodDef spkmeansmoduleMethods[] = {
    {"wam",
     (PyCFunction)wam,
     METH_VARARGS,                                                               /* flags indicating parameters
                                                   accepted for this function */
     PyDoc_STR("Calculates the weighted adjacency matrix of a given function")}, /*  The docstring for the function */
    {"ddg",
     (PyCFunction)ddg,
     METH_VARARGS,                                                   /* flags indicating parameters
                                       accepted for this function */
     PyDoc_STR("Calculates the degree matrix of a given function")}, /*  The docstring for the function */
    {NULL, NULL, 0, NULL}                                            /* The last entry must be all NULL as shown to act as a
                                                                        sentinel. Python looks for this entry to know that all
                                                                        of the functions for the module have been defined. */
};

static struct PyModuleDef spkmeansmoduleModule = {
    PyModuleDef_HEAD_INIT,
    "spkmeansmodule",     /* name of module */
    NULL,                 /* module documentation, may be NULL */
    -1,                   /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    spkmeansmoduleMethods /* the PyMethodDef array from before containing the methods of the extension */
};

PyMODINIT_FUNC PyInit_spkmeansmodule(void)
{
    PyObject *m;
    m = PyModule_Create(&spkmeansmoduleModule);
    if (!m)
    {
        return NULL;
    }
    return m;
}