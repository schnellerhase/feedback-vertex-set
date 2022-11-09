#pragma once

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <numeric>
#ifdef __unix__
#include <unistd.h>
#elif __MSDOS__ || __WIN32__ || _MSC_VER
#include <io.h>
#endif
#include "util/types.hpp"
#include "vertex_marker.hpp"

using VC = VertexMarker;

class VCFunnelHandler
{

  private:
    IndexListList _v;
    IndexListList _vInvert;
    IndexListList _u;
    IndexListList _uInvert;

    IndexListList _vNeighbors;
    IndexListList _uNeighbors;

  public:
    VCFunnelHandler()
      : _v(IndexListList())
      , _vInvert(IndexListList())
      , _u(IndexListList())
      , _uInvert(IndexListList())
      , _vNeighbors(IndexListList())
      , _uNeighbors(IndexListList())
    {
    }

    VCFunnelHandler(IndexListList v,
                    IndexListList vInvert,
                    IndexListList u,
                    IndexListList uInvert,
                    IndexListList vNeighbors,
                    IndexListList uNeighbors)
      : _v(std::move(v))
      , _vInvert(std::move(vInvert))
      , _u(std::move(u))
      , _uInvert(std::move(uInvert))
      , _vNeighbors(std::move(vNeighbors))
      , _uNeighbors(std::move(uNeighbors))
    {
    }

    explicit VCFunnelHandler(const VCFunnelHandler& other) =
      default; // copy should not happen uintended
    VCFunnelHandler& operator=(const VCFunnelHandler& other) =
      delete; // if you need to copy -> must use explicit copy constructor

    VCFunnelHandler(VCFunnelHandler&& other) = default;
    VCFunnelHandler& operator=(VCFunnelHandler&& other) = default;

    ~VCFunnelHandler() = default;

    [[nodiscard]] const IndexListList& v() const { return _v; }

    [[nodiscard]] const IndexListList& vInvert() const { return _vInvert; }

    [[nodiscard]] const IndexListList& vNeighbors() const
    {
        return _vNeighbors;
    }

    [[nodiscard]] const IndexListList& u() const { return _u; }

    [[nodiscard]] const IndexListList& uInvert() const { return _uInvert; }

    [[nodiscard]] const IndexListList& uNeighbors() const
    {
        return _uNeighbors;
    }

    void update(IndexList v,
                IndexList vInvert,
                IndexList u,
                IndexList uInvert,
                IndexList vNeighbors,
                IndexList uNeighbors) // TODO Do not copy
    {
        _u.push_back(u);
        _uInvert.push_back(uInvert);
        _v.push_back(v);
        _vInvert.push_back(vInvert);
        _uNeighbors.push_back(uNeighbors);
        _vNeighbors.push_back(vNeighbors);
        return;
    }

    void update_vc(VC& vc, index_t i)
    {
        assert(i < _u.size());
        bool vNeighborsInVc = true;

        for (auto vNeighbor : _vNeighbors[i]) {
            vNeighborsInVc &= vc[vNeighbor];
            if (!vNeighborsInVc)
                break;
        }

        if (vNeighborsInVc) {
            for (auto uParent : _u[i])
                vc[uParent] = true;

            for (auto vParentInvert : _vInvert[i])
                vc[vParentInvert] = true;
        }

        else {
            for (auto vParent : _v[i])
                vc[vParent] = true;

            for (auto uParentInvert : _uInvert[i])
                vc[uParentInvert] = true;
        }
    }

    void update_vc_all(VC& vc)
    {
        for (index_t i(_u.size() - 1); (i + 1) >= 1; i--) {
            update_vc(vc, i);
        }
    }

    bool operator==(const VCFunnelHandler& rhs) const
    {
        return (_u == rhs._u) && (_uInvert == rhs._uInvert) && (_v == rhs._v) &&
               (_vInvert == rhs._vInvert) && (_uNeighbors == rhs._uNeighbors) &&
               (_vNeighbors == rhs._vNeighbors);
    }

    // bool operator[](index_t index) const
    // {
    //     if (index >= _fvs.size()) {
    //         std::cout << "FVS funnel index out of bound, exiting";
    //         exit(0);
    //     }
    //     return _fvs[index];
    // }

    // bool& operator[](index_t index)
    // {
    //     // if (index >= _fvs.size()) {
    //     //     std::cout << "FVS funnel index out of bound, exiting";
    //     //     exit(0);
    //     // }
    //     return _fvs[index];
    // }

    // const std::vector<bool> &get_fvs() const
    // {
    //     return _fvs;

    // }

    // index_t size() const
    // {
    //     return _fvs.size();

    // }
};

std::ostream&
operator<<(std::ostream& os, const VCFunnelHandler& vcHandler)
{
    os << "v =" << vcHandler.v() << "\n"
       << "vInvert =" << vcHandler.vInvert() << "\n"
       << "u =" << vcHandler.u() << "\n"
       << "uInvert =" << vcHandler.uInvert() << "\n"
       << "vNeighbors =" << vcHandler.vNeighbors() << "\n"
       << "uNeighbors =" << vcHandler.uNeighbors() << "\n";
    return os;
}
