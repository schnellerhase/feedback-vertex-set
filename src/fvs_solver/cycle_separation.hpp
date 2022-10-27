#pragma once

#include <algorithm>
#include <iostream>
#include <list>
#include <queue>
#include <stack>
#include <vector>

#include "discrete/discrete.hpp"

struct MinHeapDistCompare
{
    double* const _dist;

    MinHeapDistCompare(double* d)
      : _dist(d)
    {
    }

    // Need to have a greater rather than less style because
    // std::heap is internally a max heap
    bool operator()(const int& one, const int& two) const
    {
        return _dist[one] > _dist[two];
    }
};

struct LessCompare
{
    double* const _value;

    LessCompare(double* v)
      : _value(v)
    {
    }

    bool operator()(const int& one, const int& two) const
    {
        return _value[one] < _value[two];
    }
};

struct LessCompare2
{
    int* const _value;

    LessCompare2(int* v)
      : _value(v)
    {
    }

    bool operator()(const int& one, const int& two) const
    {
        return _value[one] < _value[two];
    }
};

struct GreaterCompare
{
    double* const _value;

    GreaterCompare(double* v)
      : _value(v)
    {
    }

    bool operator()(const int& one, const int& two) const
    {
        return _value[one] > _value[two];
    }
};

struct CycleSeparation
{
    const Graph& _data;

    int* const _pv;
    int* const _pe;
    int* const _pl;
    double* const _d;
    double* const _w;

    MinHeapDistCompare* const _hcomp;
    int* const _heap;
    int* const _heapid;
    int _heaped;
    int _heaprun;
    const double _infty;
    const double _eps;
    index_t _nCuts;

    int** _cutsupport;
    index_t* const _cutlens;

    CycleSeparation(const Graph& d)
      : _data(d)
      , _pv(new int[d.N()])
      , _pe(new int[d.N()])
      , _pl(new int[d.N()])
      , _d(new double[d.N()])
      , _w(new double[d.M()])
      , _hcomp(new MinHeapDistCompare(_d))
      , _heap(new int[d.M()])
      , _heapid(new int[d.M()])
      , _heaped(0)
      , _heaprun(0)
      , _infty(d.N() << 1)
      , _eps(1e-6)
      , _nCuts(0)
      , _cutsupport(new int*[d.M()])
      , _cutlens(new index_t[d.M()])
    {
        for (index_t i = 0; i < _data.M(); ++i) {
            _cutsupport[i] = 0;
        }
    }

    int separate(double* x)
    {
        for (index_t e = 0; e < _data.M(); ++e) {
            _w[e] = (1.0 - x[e]);
            //        std::cout << "weight of edge " << e << ": " << _w[e] <<
            //        std::endl;
        }

        // Cleanup of previous calls
        while (_nCuts) {
            --_nCuts;
            delete[] _cutsupport[_nCuts];
            _cutsupport[_nCuts] = 0;
            _cutlens[_nCuts] = 0;
        }

        for (index_t s = 0; s < _data.N(); ++s) {
            // for all arcs (t,s)
            // check whether this arc together with
            // the shortest s-t path gives a violated constraint.

            // Thereby, try to reuse distance labels for different t
            // It might be that d[t] has been computed in a run before
            // namely if d[t] != infty and _heapid[t] == -1
            // i.e., t has been removed from the heap already

            for (index_t i = 0; i < _data.N(); ++i) {
                _d[i] = _infty;
                _pv[i] = _infty;
                _pl[i] = _infty;
                _heapid[i] = 0;
            }

            _heaprun = 0;
            _heaped = 0;

            const int start = s;
            _d[start] = 0.0;
            _pv[start] = 0;
            _pl[start] = 0;

            _heap[_heaped] = start;
            ++_heaped;
            _heapid[start] = _heaprun;
            ++_heaprun;

            std::make_heap(_heap, _heap + _heaped, (*_hcomp));

            // Enhancement: To speed up and to avoid finding
            // cycles more than once, we only follow paths
            // where s is the smallest node index (for each
            // cycle there must be one smallest index...)

            for (index_t k = 0; k < _data.indeg()[s]; ++k) {
                const index_t t = _data.inadj()[s][k];

                if (t < s)
                    continue;

                double w_cl = _w[_data.in2arc()[s][k]];

                if (w_cl + _eps > 1.0)
                    continue;

                // check if (final) d[t] is already known
                if (_d[t] < _infty && _heapid[t] < 0) {
                    if (_d[t] + w_cl + _eps < 1.0) {
                        // Found violated cycle

                        _cutsupport[_nCuts] = new int[_pl[t] + 1];
                        _cutlens[_nCuts] = _pl[t] + 1;

                        index_t v = t;

                        //                  std::cout << "Cycle [" << t << ","
                        //                  << s << "]  with viol: " << _d[t] +
                        //                  w_cl << std::endl;

                        //                        std::cout << "Cycle [" << t <<
                        //                        "," << s << "] = ";

                        // walk cycle backwards (just for outputting edges!)
                        while (v != s) {
                            //                            std::cout << _pl[v] <<
                            //                            ": " <<_pe[v] << " ("
                            //                            <<
                            //                            _data.tails()[_pe[v]]
                            //                            << "," <<
                            //                            _data.heads()[_pe[v]]
                            //                            << "),  ";
                            _cutsupport[_nCuts][_pl[v]] = _pe[v];
                            v = _pv[v];
                        }

                        _cutsupport[_nCuts][0] = _data.in2arc()[s][k];

                        //                        std::cout <<  "0: " <<
                        //                        _data.in2arc()[s][k] << " ("
                        //                        <<
                        //                        _data.tails()[_data.in2arc()[s][k]]
                        //                        << "," <<
                        //                        _data.heads()[_data.in2arc()[s][k]]
                        //                        << ")" << std::endl;

                        ++_nCuts;
                    }

                    continue;
                }

                while (_heaped != 0) {
                    index_t node = _heap[0];
                    std::pop_heap(_heap, _heap + _heaped, (*_hcomp));
                    --_heaped;

                    _heapid[node] = -1;

                    if (_d[node] + _eps > 1.0) {
                        // "clear" the heap in order to set all heapids to < 0
                        continue;
                    }

                    //                std::cout << "At node " << node <<
                    //                std::endl;

                    // It might be that node == t but this is handled
                    // below to handle loop breaking more easily.
                    for (index_t l = 0; l < _data.outdeg()[node]; ++l) {
                        const index_t j = _data.outadj()[node][l];

                        if (j < s)
                            continue;

                        const double ndistj =
                          _w[_data.out2arc()[node][l]] + _d[node];

                        //                    std::cout << "ndist for " << j <<
                        //                    ": " << ndistj << " and d of it
                        //                    now " << _d[j] << std::endl;

                        if (ndistj >= 1.0) {
                            //                        std::cout << "ndist for "
                            //                        << j << " before placement
                            //                        on heap" << ndistj <<
                            //                        std::endl;
                            continue;
                        }

                        if (_heapid[j] >= 0 &&
                            ((ndistj < _d[j]) ||
                             (ndistj <= _d[j] && _pl[node] + 1 < _pl[j]))) {
                            _d[j] = ndistj;
                            _pv[j] = node;
                            _pe[j] = _data.out2arc()[node][l];
                            _pl[j] = _pl[node] + 1;

                            if (_heapid[j] == 0) {
                                //                            std::cout <<
                                //                            "Adding vertex "
                                //                            << j << " to heap
                                //                            with dist " <<
                                //                            _d[j] << ", plen "
                                //                            << _pl[node] + 1
                                //                            /*<< " and
                                //                            incoming edge " <<
                                //                            e << " = {" <<
                                //                            _tail[e] << "," <<
                                //                            _head[e] << "}" */
                                //                            << std::endl;

                                _heap[_heaped] = j;
                                ++_heaped;

                                std::push_heap(
                                  _heap, _heap + _heaped, (*_hcomp));

                                _heapid[j] = _heaprun;
                                ++_heaprun;
                            } else {
                                // Decrease Key workaround
                                if (!std::is_heap(
                                      _heap, _heap + _heaped, (*_hcomp)))
                                    std::make_heap(
                                      _heap, _heap + _heaped, (*_hcomp));

                                //                            std::cout <<
                                //                            "Changing key
                                //                            vertex " << j << "
                                //                            in heap with dist
                                //                            " << _d[j] << ",
                                //                            plen " <<
                                //                            _pl[node] + 1 /*<<
                                //                            " and incoming
                                //                            edge " << e << " =
                                //                            {" << _tail[e] <<
                                //                            "," << _head[e] <<
                                //                            "}" */ <<
                                //                            std::endl;
                            }
                        }
                    }

                    if (node == t) {
                        //                    std::cout << "Shortest [" << t <<
                        //                    "," << s << "] cycle has length: "
                        //                    << _d[node] + w_cl << std::endl;

                        if (_d[node] + w_cl + _eps < 1.0) {
                            // Found violated cycle

                            _cutsupport[_nCuts] = new int[_pl[node] + 1];
                            _cutlens[_nCuts] = _pl[node] + 1;

                            index_t v = node;

                            //                        std::cout << "Cycle [" <<
                            //                        t << "," << s << "]  with
                            //                        viol: " << _d[node] + w_cl
                            //                        << std::endl;

                            //                        std::cout << "Cycle [" <<
                            //                        t << "," << s << "] = ";

                            // walk cycle backwards (just for outputting edges!)
                            while (v != s) {
                                //                            std::cout <<
                                //                            _pl[v] << ": "
                                //                            <<_pe[v] << " ("
                                //                            <<
                                //                            _data.tails()[_pe[v]]
                                //                            << "," <<
                                //                            _data.heads()[_pe[v]]
                                //                            << "),  ";
                                _cutsupport[_nCuts][_pl[v]] = _pe[v];
                                v = _pv[v];
                            }

                            _cutsupport[_nCuts][0] = _data.in2arc()[s][k];

                            //                        std::cout <<  "0: " <<
                            //                        _data.in2arc()[s][k] << "
                            //                        (" <<
                            //                        _data.tails()[_data.in2arc()[s][k]]
                            //                        << "," <<
                            //                        _data.heads()[_data.in2arc()[s][k]]
                            //                        << ")" << std::endl;

                            ++_nCuts;
                        }

                        break;
                    }
                }
            }
        }

        return _nCuts;
    }

    bool check(double* x)
    {
        for (index_t e = 0; e < _data.M(); ++e) {
            _w[e] = (1.0 - x[e]);
            //        std::cout << "weight of edge " << e << ": " << _w[e] <<
            //        std::endl;
        }

        for (index_t s = 0; s < _data.N(); ++s) {
            // for all arcs (t,s)
            // check whether this arc together with
            // the shortest s-t path gives a violated constraint.

            // Thereby, try to reuse distance labels for different t
            // It might be that d[t] has been computed in a run before
            // namely if d[t] != infty and _heapid[t] == -1
            // i.e., t has been removed from the heap already

            for (index_t i = 0; i < _data.N(); ++i) {
                _d[i] = _infty;
                _pv[i] = _infty;
                _pl[i] = _infty;
                _heapid[i] = 0;
            }

            _heaprun = 0;
            _heaped = 0;

            const int start = s;
            _d[start] = 0.0;
            _pv[start] = 0;
            _pl[start] = 0;

            _heap[_heaped] = start;
            ++_heaped;
            _heapid[start] = _heaprun;
            ++_heaprun;

            std::make_heap(_heap, _heap + _heaped, (*_hcomp));

            // We could and should certainly pre-compute
            // whether (t,s) is at all part of a cycle
            for (index_t k = 0; k < _data.indeg()[s]; ++k) {
                const int t = _data.inadj()[s][k];

                double w_cl = _w[_data.in2arc()[s][k]];

                if (w_cl + _eps > 1.0)
                    continue;

                // check if (final) d[t] is already known
                if (_d[t] < _infty && _heapid[t] < 0) {
                    if (_d[t] + w_cl + _eps < 1.0) {
                        // Found violated cycle

                        return false;
                    }
                }

                while (_heaped != 0) {
                    int node = _heap[0];
                    std::pop_heap(_heap, _heap + _heaped, (*_hcomp));
                    --_heaped;

                    _heapid[node] = -1;

                    if (_d[node] + _eps > 1.0) {
                        // "clear" the heap in order to set all heapids to < 0
                        continue;
                    }

                    //                std::cout << "At node " << node <<
                    //                std::endl;

                    // It might be that node == t but this is handled
                    // below to handle loop breaking more easily.
                    for (index_t l = 0; l < _data.outdeg()[node]; ++l) {
                        const int j = _data.outadj()[node][l];

                        const double ndistj =
                          _w[_data.out2arc()[node][l]] + _d[node];

                        //                    std::cout << "ndist for " << j <<
                        //                    ": " << ndistj << " and d of it
                        //                    now " << _d[j] << std::endl;

                        if (_heapid[j] >= 0 &&
                            ((ndistj < _d[j]) ||
                             (ndistj <= _d[j] && _pl[node] + 1 < _pl[j]))) {
                            _d[j] = ndistj;
                            _pv[j] = node;
                            _pe[j] = _data.out2arc()[node][l];
                            _pl[j] = _pl[node] + 1;

                            if (!_heapid[j]) {
                                //                            std::cout <<
                                //                            "Adding vertex "
                                //                            << j << " to heap
                                //                            with dist " <<
                                //                            _d[j] << ", plen "
                                //                            << _pl[node] + 1
                                //                            /*<< " and
                                //                            incoming edge " <<
                                //                            e << " = {" <<
                                //                            _tail[e] << "," <<
                                //                            _head[e] << "}" */
                                //                            << std::endl;

                                _heap[_heaped] = j;
                                ++_heaped;

                                std::push_heap(
                                  _heap, _heap + _heaped, (*_hcomp));

                                _heapid[j] = _heaprun;
                                ++_heaprun;
                            } else {
                                // Decrease Key workaround
                                if (!std::is_heap(
                                      _heap, _heap + _heaped, (*_hcomp)))
                                    std::make_heap(
                                      _heap, _heap + _heaped, (*_hcomp));

                                //                            std::cout <<
                                //                            "Changing key
                                //                            vertex " << j << "
                                //                            in heap with dist
                                //                            " << _d[j] << ",
                                //                            plen " <<
                                //                            _pl[node] + 1 /*<<
                                //                            " and incoming
                                //                            edge " << e << " =
                                //                            {" << _tail[e] <<
                                //                            "," << _head[e] <<
                                //                            "}" */ <<
                                //                            std::endl;
                            }
                        }
                    }

                    if (node == t) {
                        //                    std::cout << "Shortest [" << t <<
                        //                    "," << s << "] cycle has length: "
                        //                    << _d[node] + w_cl << std::endl;

                        if (_d[node] + w_cl + _eps < 1.0) {
                            // Found violated cycle

                            return false;
                        }

                        break;
                    }
                }
            }
        }

        return true;
    }

    ~CycleSeparation()
    {
        while (_nCuts) {
            --_nCuts;
            delete[] _cutsupport[_nCuts];
        }

        delete[] _cutsupport;
        delete[] _cutlens;

        delete[] _heapid;

        delete[] _heap;
        delete _hcomp;
        delete[] _w;
        delete[] _pv;
        delete[] _pe;
        delete[] _pl;
        delete[] _d;
    }
};
