/*!
 * \file      computePatchOpt.cpp
 * \author    Jérémie Piellard
 * \author    Léo Gouttefarde
 * \date      November, 2015
 * \brief     Implements the computePatchOpt program.
 * \warning   Usage: computePatchOpt originalFile patchedFile
 * \copyright GNU Public License.
 *
 * This is the computePatchOpt main program, which computes an optimal patch.
 *
 *
 * \mainpage Documentation du TP AOD
 *
 * Documentation du TP AOD de génération de patch optimal.
 *
 * Pour lire la documentation, parcourir les différentes classes ou fichiers
 * dans l'arborescence ou faire une recherche.
 *
 */

#include "solver.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdexcept>

#ifdef PAPI
#include <stdio.h>
#include <papi.h>
#endif

using namespace std;

/*!
 * \brief     Main program
 * \param   argc    Number of arguments
 * \param   argv    Arguments
 */
int main (int argc, char **argv) {
    try {
        if (argc < 3)
            throw runtime_error ("Usage : " + string(argv[0])
                                            + " <originalFile> <patchedFile>");

        #ifdef PAPI
        long long counters[3];
        int PAPI_events[] = {
            PAPI_TOT_CYC,
            PAPI_L2_DCM,
            PAPI_L2_DCA };

        PAPI_library_init(PAPI_VER_CURRENT);
        const int i = PAPI_start_counters( PAPI_events, 3 );
        (void)i;
        #endif

        const clock_t start = clock();

        /* Allocation dynamique pour avoir la durée totale
         * (sans oublier la désallocation mémoire) */
        Solver *solver = new Solver(argv[1], argv[2]);

        cerr << "Calcul des couts..." << endl;

        solver->compute_costs(true);

        cerr << "Calcul termine !" << endl;
        cerr << "Cout total : " << solver->get_min_cost() << endl;

        cerr << endl << "Ecriture du patch..." << endl;

        solver->display_solution();

        cerr << "Ecriture terminee !" << endl << endl;

        delete solver;


        const clock_t diff = clock() - start;
        cerr << "Duree totale : " << (diff * 1000) / CLOCKS_PER_SEC
             << " millisecondes !" << endl;


        #ifdef PAPI
        PAPI_read_counters( counters, 3 );

        printf("%lld L2 cache misses (%.3lf%% misses) in %lld cycles\n", 
            counters[1],
            (double)counters[1] / (double)counters[2],
            counters[0] );
        #endif
    }
    catch (const std::exception &excep) {
        cerr << endl << "Erreur : " << excep.what() << endl << endl;
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
