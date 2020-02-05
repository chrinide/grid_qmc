#include <iostream>
#include "ref_qhamiltonian.h"
#include "qhamiltonian.h"
#include "qparams_reader.h"
#include "qintegral.h"
#include "qorbitals.h"
#include <map>
#include <string>
#include <cassert>
#include <vector>


void print_vector(const std::vector<size_t> &v) {
    for (const auto &e : v)
        std::cout << e << '\t';
    std::cout << std::endl;
}


int main(int argc, char **argv) {

    std::map<std::string, int> params {{"Z", 2}, {"nrad", 10}, {"nang", 14}, {"L_max", 0}, {"mult", 3}, {"electrons", 2}};
    assert (params["electrons"] == 2);

    ShellSet ss(params["L_max"]);
    Grid_integrals ig(params, ss); // Will create the grid internally

    assert (ig.n1porb == 10);

    REF::DetBasis ref_bas(params, ig.n1porb);
    DetBasis bas(params, ig.n1porb);

    // Perform a series of simple tests to make sure that the bases are
    // essentially the same

    // Number of alpha and beta electrons in the  basis
    auto [ref_na, ref_nb ] = ref_bas.get_ab();
    auto [na, nb ] = bas.get_ab();
    assert ( na == ref_na && nb == ref_nb);

    // Number of alpha and beta strings
    auto [nalp, nbet] = bas.get_num_str();
    auto [ref_nalp, ref_nbet] = ref_bas.get_num_str();
    assert (ref_nalp == nalp && ref_nbet == nbet);

    // Size of  the basis set
    auto ref_bas_size = ref_bas.get_basis_size(), bas_size = bas.get_basis_size();
    assert (ref_bas_size == bas_size);

    // Loop over the ref basis and print str

    std::cout << " Basis from the reference class " << std::endl;

    for (size_t i = 0; i < ref_bas_size; i++) {
        auto [ia, ib] = ref_bas.unpack_str_index(i);
        auto &a = ref_bas.a(ia);
        print_vector(a);
        if (ref_nb !=0) {
            auto &b = ref_bas.b(ib);
            print_vector(b);
        }
    }

    // Loop over the basis and print str

    std::cout << " Basis from the main distro class " << std::endl;

    for (size_t i = 0; i < bas_size; i++) {
        auto [ia, ib] = bas.unpack_str_index(i);
        {
            //std::vector<size_t> tmp;
            //tmp.resize(na);
            //std::copy(bas.a(ia).begin(), bas.a(ia).end(), tmp.begin()); // Segfaults!
            // -------------------------------------------
            //auto a = bas.b(ia);
            //print_vector(a); // Works fine
            const auto &tmp = bas.a(ia);
            print_vector(tmp);
        }
        if (nb !=0) {
            std::vector<size_t> tmp;
            tmp.resize(nb);
            std::copy(bas.b(ib).begin(), bas.b(ib).end(), tmp.begin());
            //auto b = bas.b(ib);
            print_vector(tmp);
        }
    }

    return 0;

}

