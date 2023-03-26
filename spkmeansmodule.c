#include <Python.h>
#include "matrix.h"
#include "jacobi.h"
#include "kmeans.h"

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

static PyObject *wrapEigenParamsWithPyObject(eigenParam **eigen_params, int n)
{
    PyObject *lst = PyList_New(n + 1);

    PyObject *eigenvalues = PyList_New(n);
    for (int i = 0; i < n; i++)
    {
        PyList_SetItem(eigenvalues, i, Py_BuildValue("d", eigen_params[i]->eigen_value));
    }
    PyList_SetItem(lst, 0, eigenvalues);

    for (int i = 0; i < n; i++)
    {
        PyObject *eigenvector = PyList_New(n);
        for (int j = 0; j < n; j++)
        {
            PyList_SetItem(eigenvector, j, Py_BuildValue("d", eigen_params[j]->eigen_vector[i]));
        }

        PyList_SetItem(lst, i + 1, eigenvector);
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

static PyObject *gl(PyObject *self, PyObject *args)
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
    double **wam = createWeightedAdjMatrix(parsed_data_points, num_of_points, num_of_cords);
    double **ddg = createDegMatrix(wam, num_of_points);

    return wrapWithPyObject(createGraphLaplacian(wam, ddg, num_of_points),
                            num_of_points,
                            num_of_points);
}

static PyObject *jacobi_wrap(PyObject *self, PyObject *args)
{
    PyObject *data_points;
    double **parsed_data_points;

    if (!PyArg_ParseTuple(args, "O", &data_points))
    {
        return NULL;
    }

    parsed_data_points = parseDataPoints(data_points);
    int num_of_points = PyObject_Length(data_points);

    return wrapEigenParamsWithPyObject(jacobi(parsed_data_points, num_of_points),
                                       num_of_points);
}

static PyMethodDef spkmeansmoduleMethods[] = {
    {"wam",
     (PyCFunction)wam,
     METH_VARARGS,                                                                          /* flags indicating parameters
                                                              accepted for this function */
     PyDoc_STR("Calculates the weighted adjacency matrix for a given set of data points")}, /*  The docstring for the function */

    {"ddg",
     (PyCFunction)ddg,
     METH_VARARGS,
     PyDoc_STR("Calculates the degree matrix for a given set of data points")},

    {"gl",
     (PyCFunction)gl,
     METH_VARARGS,
     PyDoc_STR("Calculates the graph laplacian for a given set of data points")},

    {"jacobi",
     (PyCFunction)jacobi_wrap,
     METH_VARARGS,
     PyDoc_STR("Calculates the eigenvalues and eigenvectors for a given matrix according to Jacobi method")},

    {"spk",                   
      (PyCFunction) fit, 
      METH_VARARGS,           
      PyDoc_STR("A kmeans implemention written in C")},

    {NULL, NULL, 0, NULL} /* The last entry must be all NULL as shown to act as a
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