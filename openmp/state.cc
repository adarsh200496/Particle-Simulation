#include <algorithm>
#include "state.h"

namespace state {

  template<typename Real>
    /// initialize the state vectors
    State<Real>::State(size_t n_charges_per_dir)
    {
      size_t n_c = n_charges_per_dir;

      _n_charges = n_c * n_c * n_c;

      _x = new Real[_n_charges]; _y = new Real[_n_charges]; _z = new Real[_n_charges];
      _u = new Real[_n_charges]; _v = new Real[_n_charges]; _w = new Real[_n_charges];

      if (n_c == 0) {
        return;
      }

      if (n_c == 1) {
        // one particle: at the origin
        _x[0] = _y[0] = _z[0] = 0;
      } else {
        // otherwise regularly spaced on [-1, 1]^3
        Real incr = ((Real) 2) / (n_c - 1);

        size_t i, j, k = 0;
        #pragma omp parallel for collapse(3)
        for (size_t i = 0, index = 0; i < n_c; i++) {
          for (size_t j = 0; j < n_c; j++) {
            for (size_t k = 0; k < n_c; k++, index++) {
              _x[index] = std::min(((Real) -1) + incr * i, (Real) 1);
              _y[index] = std::min(((Real) -1) + incr * j, (Real) 1);
              _z[index] = std::min(((Real) -1) + incr * k, (Real) 1);
            }
          }
        }
      }

      // all velocities initially zero
      size_t i = 0;
      #pragma omp parallel for shared (_n_charges, _u, _v, _w) private (i)
      for (size_t i = 0; i < _n_charges; i++) {
        _u[i] = _v[i] = _w[i] = 0;
      }
    }

  template<typename Real>
    size_t State<Real>::n_charges()
    {
      return _n_charges;
    }

  template<typename Real>
      void State<Real>::arrays(Real *x[], Real *y[], Real *z[], Real *u[], Real *v[], Real *w[])
      {
        *x = _x;
        *y = _y;
        *z = _z;
        *u = _u;
        *v = _v;
        *w = _w;
      }

  template<typename Real>
    State<Real>::~State()
    {
      delete [] _w;
      delete [] _v;
      delete [] _u;
      delete [] _z;
      delete [] _y;
      delete [] _x;
    }
}
