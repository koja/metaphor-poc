#include "../../include/harel_koren_3d.hpp"

#include "graph_topology.hpp"
#include "../util/euclidean.hpp"
#include "../util/math.hpp"
#include "../util/random.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <list>
#include <queue>
#include <tuple>
#include <utility>
#include <vector>

std::vector<V_Id> get_cluster_centers(const std::size_t k, const Distance_Matrix& shortest_paths) {

    const std::size_t N = get_N(shortest_paths);
    // TODO assert k < N
    std::vector<V_Id> centers(k);
    std::vector<bool> is_center(N, false);

    std::vector<Topo_Dist> vertex2nearest_center_dist(N);
    // vertexes ordered by nearest center dist
    std::map<Topo_Dist, std::vector<V_Id>> nearest_center_dist2vertexes;

    // Select the first center randomly.
    {
        centers.at(0) = get_random_in_0_max( get_N(shortest_paths) );
        is_center.at(centers.at(0)) = true;

        for(V_Id u = 0; u < N; u++) {
            nearest_center_dist2vertexes[ get_val( shortest_paths, u, centers.at(0) ) ].emplace_back(u);
            vertex2nearest_center_dist.at(u) = get_val( shortest_paths, u, centers.at(0) );
        }
    }

    // Select the remaining k-1 centers.
    std::vector<std::size_t> other_center_candidates;

    for(std::size_t i = 1; i < k; i++) {

        // Find the vertex which is most (topologically) distant from all current centers (there can be more than one).
        std::vector<V_Id> new_center_candidates;
        auto nearest_candidate_vertexes_it = nearest_center_dist2vertexes.rbegin();
        while(true) {
            for( const V_Id v : nearest_candidate_vertexes_it->second ) {
                if( !is_center[v] ) {
                    new_center_candidates.push_back(v);
                }
            }

            if( !new_center_candidates.empty() ) { break; }
            if( nearest_candidate_vertexes_it == nearest_center_dist2vertexes.rend() ) { throw std::runtime_error("seems like no not-yet-center vertex is nearby"); }
            ++nearest_candidate_vertexes_it;
        };

        // From these candidate vertices (which are all equally good candidates), select the new center randomly.
        const V_Id new_center = new_center_candidates[ get_random_in_0_max(new_center_candidates.size() - 1) ];

        centers.at(i) = new_center;
        is_center.at(new_center) = true;

        // Update dist_from_center to take the new center into account.
        for(V_Id u = 0; u < N; u++) {

            const Topo_Dist current_dist = vertex2nearest_center_dist.at(u);
            const Topo_Dist dist_to_new_center = get_val(shortest_paths, u, new_center);
            if(dist_to_new_center < current_dist) {
                vertex2nearest_center_dist.at(u) = dist_to_new_center;
                std::remove(
                    nearest_center_dist2vertexes.at(current_dist).begin(),
                    nearest_center_dist2vertexes.at(current_dist).end(),
                    u
                );
                nearest_center_dist2vertexes.at(dist_to_new_center).push_back(u);
            }
        }
    }
    return centers;
}

/**
 * @return random clusters of radius max_dist_to_cluster_center around cluster_centers
 */
std::vector<position3> create_random_clusters(
    const std::vector<position3>& current_positions,
    const std::vector<V_Id>& cluster_centers,
    const Distance_Matrix& topological_shortest_paths,
    const double max_dist_to_cluster_center
) {

    std::vector<position3> result;

    for(V_Id v = 0; v < current_positions.size(); v++) {
        // TODO optimalizace - postavit z cluster_centers_indexes nejakou strukturu pro rychlejsi hledani?
        // for center just copy position
        if( std::find(cluster_centers.begin(), cluster_centers.end(), v) != cluster_centers.end() ) {
            result.emplace_back( current_positions.at(v) );
            continue;
        }

        V_Id nearest_center = 0;
        {
            Topo_Dist current_shortest_dist = get_val( topological_shortest_paths, v, cluster_centers.at(0) );

            for(const V_Id center : cluster_centers) {

                const Topo_Dist d_v_center = get_val( topological_shortest_paths, v, center );

                if(d_v_center < current_shortest_dist) {
                    current_shortest_dist = d_v_center;
                    nearest_center = center;
                }
            }
        }

        // TODO proc to neni homogenni rozmisteni?
        // TODO: upgrade - don't allow to be positioned exactly as center - make minimum of 2 * eps
        const double dist_x = get_random_double_0_1() * max_dist_to_cluster_center;
        const double dist_y = get_random_double_0_1() * max_dist_to_cluster_center;
        const double dist_z = get_random_double_0_1() * max_dist_to_cluster_center;

        result.emplace_back(
            current_positions.at(nearest_center).x + get_random_sign() * dist_x,
            current_positions.at(nearest_center).y + get_random_sign() * dist_y,
            current_positions.at(nearest_center).z + get_random_sign() * dist_z
        );
    }

    return result;
}

std::vector<double> compute_grad_E_norm(
    const std::vector<V_Id>& verts,
    const Distance_Matrix& shortPath,
    const std::vector<position3>& L,
    const std::map<Vert_Idx, std::set<Vert_Idx>>& close_neighbours
) {
    std::vector<first_order_derivation3> dE(
        verts.size(),
        first_order_derivation3(0.0, 0.0, 0.0)
    );

    for(size_t i = 0; i < verts.size(); i++) {
        const V_Id v = verts.at(i);
        for(const Vert_Idx j : close_neighbours.at(i)) {
            if(i == j) continue;
            const V_Id u = verts.at(j);

            const double dx = L.at(u).x - L.at(v).x;
            const double dy = L.at(u).y - L.at(v).y;
            const double dz = L.at(u).z - L.at(v).z;

            const double d_T = static_cast<double>( get_val(shortPath, v, u) );
            const double d_E = get_E_dist(L.at(u), L.at(v));

            const double t = 1.0/non_zero(d_T) - 1.0/non_zero(d_E);

            // NOTE: I am skipping factor of 2* and dealing with it when actually computing Euclidean norm ...
            dE.at(i).dx -= t * dx; dE.at(j).dx += t * dx;
            dE.at(i).dy -= t * dy; dE.at(j).dy += t * dy;
            dE.at(i).dz -= t * dz; dE.at(j).dz += t * dz;
        }
    }

    std::vector<double> gradE(verts.size());

    for(V_Id i = 0; i < verts.size(); i++) {
        gradE.at(i) =
            (
                4 * (   // NOTE: ... here the missing factor of 2 (squared)
                    dE.at(i).dx * dE.at(i).dx + dE.at(i).dy * dE.at(i).dy + dE.at(i).dz * dE.at(i).dz
                )
            );
    }

    return gradE;
};

// TODO optimalizace - prvni derivace se pocitaji uz pri zjistovani grad_E_norm()
// @returns dEdx, dEdy
std::pair<first_order_derivation3, second_order_derivation3> compute_derivatives_of_E(
    const std::vector<V_Id>& verts,
    const Distance_Matrix& shortPath,
    const V_Id u,
    const std::set<Vert_Idx>& close_neighbours_of_u,
    std::vector<position3>& L
) {
    first_order_derivation3 dE{0.0, 0.0, 0.0};
    second_order_derivation3 d2E{0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

    for(const Vert_Idx v_idx : close_neighbours_of_u) {
        V_Id v = verts.at(v_idx);
        if(v == u) { continue; }

        const double dx = L.at(u).x - L.at(v).x;
        const double dy = L.at(u).y - L.at(v).y;
        const double dz = L.at(u).z - L.at(v).z;

        // 1 / |u-v|_E ... Euler metric
        const double inv_D_E = 1.0 / get_E_dist( L.at(u), L.at(v) );
        // 1 / ( |u-v|_E )^3 ... Euler metric
        const double inv_D_E3 = inv_D_E * inv_D_E * inv_D_E;
        // 1 / |u-v|_T ... graph topological metric
        const double inv_D_T = 1.0 / static_cast<double>( get_val(shortPath, u, v) );

        const double inv_D_T_inv_D_E_diff = inv_D_T - inv_D_E;

        dE.dx    -= 2.0* dx * inv_D_T_inv_D_E_diff;
        dE.dy    -= 2.0* dy * inv_D_T_inv_D_E_diff;
        dE.dz    -= 2.0* dz * inv_D_T_inv_D_E_diff;

        d2E.dx2  -= 2.0 * inv_D_T_inv_D_E_diff  + 0.5 * dx*dx * inv_D_E3;
        d2E.dy2  -= 2.0 * inv_D_T_inv_D_E_diff  + 0.5 * dy*dy * inv_D_E3;
        d2E.dz2  -= 2.0 * inv_D_T_inv_D_E_diff  + 0.5 * dz*dz * inv_D_E3;

        d2E.dxdy -= 0.5 * dx * dy * inv_D_E3;
        d2E.dydz -= 0.5 * dy * dz * inv_D_E3;
        d2E.dzdx -= 0.5 * dz * dx * inv_D_E3;
    }

    return std::make_pair(dE, d2E);
};

void lay_out_centers(
    const std::vector<V_Id>& verts,
    std::vector<position3>& L,
    const Distance_Matrix& shortPath,
    const size_t k, // NOTE: something completely different from k clusters
    const size_t nIterations
) {
    typedef size_t Vert_Idx;

    const std::map<Vert_Idx, std::set<Vert_Idx>> close_neighbours = get_close_neighbours(verts, k, shortPath);

    for(size_t iterNo = 0; iterNo < nIterations * verts.size(); ++iterNo) {

        const std::vector<double> gradE_norm = compute_grad_E_norm(verts, shortPath, L, close_neighbours);

        // vertex where |grad E| is max
        Vert_Idx max_E_grad_idx = 0;
        for(Vert_Idx i = 0; i < verts.size(); i++) {
            if(gradE_norm.at(i) > gradE_norm.at(max_E_grad_idx)) {
                max_E_grad_idx = i;
            }
        }

        const V_Id v_of_max_E_grad = verts.at(max_E_grad_idx);

        const std::pair<first_order_derivation3, second_order_derivation3> derivatives_of_E
            = compute_derivatives_of_E(
                verts,
                shortPath,
                v_of_max_E_grad,
                close_neighbours.at(max_E_grad_idx),
                L
            );

        const second_order_derivation3& d2E = derivatives_of_E.second;

        const std::array< std::array<double, 3>, 3 > matrix{
            std::array<double, 3>{ d2E.dx2,     d2E.dxdy,   d2E.dzdx    },
            std::array<double, 3>{ d2E.dxdy,    d2E.dy2,    d2E.dydz    },
            std::array<double, 3>{ d2E.dzdx,    d2E.dydz,   d2E.dz2     }
        };

        const first_order_derivation3& dE = derivatives_of_E.first;
        const std::array<double, 3> vec{ - dE.dx, - dE.dy, - dE.dz };

        const std::array<double, 3> delta = solve_3D_linear_equation( matrix, vec );

        if( is_almost_zero(
                std::abs(delta[0]) + std::abs(delta[1]) + std::abs(delta[2])
            )
        ) {
            break;
        }

        L.at(v_of_max_E_grad).x += delta[0];
        L.at(v_of_max_E_grad).y += delta[1];
        L.at(v_of_max_E_grad).z += delta[2];
    }
}

/* TODO
static double linear_interpolation(
    const position3& a,
    const position3& b,
    const double x
) {

    if( is_almost_zero(x1 - x2) ) {
        return 0.5 * (y1 + y2);
    } else {
        return y1 + (y2 - y1) * (x - x1) / (x2 - x1);
    }
}
*/

std::unordered_map<Function_Id, position3> harel_koren_layout_3d(const Call_Graph_Intf& graph, const GraphLayoutSettings& gls) {

    std::vector<position3> positions;

    // init vertex positions with random values
    std::map<Function_Id, V_Id> function_id_to_vertex;
    std::map<V_Id, Function_Id> vertex_to_function_id;

    graph.for_each_function(
        [&function_id_to_vertex, &vertex_to_function_id, &positions](const Function& f){
            // TODO mozna jina skala?
            positions.emplace_back(
                get_random_double_0_1(), get_random_double_0_1(), get_random_double_0_1()
            );

            function_id_to_vertex[ f.id ] = V_Id{positions.size() - 1};
            vertex_to_function_id[ V_Id{positions.size() - 1} ] = f.id;
        }
    );

    const std::map<V_Id, std::set<V_Id>> edges =
        [&graph, &function_id_to_vertex]() {
            std::map<V_Id, std::set<V_Id>> tmp_edges;

            graph.for_each_call(
                [&function_id_to_vertex, &tmp_edges](const Function caller, const Function callee){
                    tmp_edges[ function_id_to_vertex.at(caller.id) ].emplace( function_id_to_vertex.at(callee.id) );
                    tmp_edges[ function_id_to_vertex.at(callee.id) ].emplace( function_id_to_vertex.at(caller.id) );
                }
            );

            return tmp_edges;
        }();

    const Distance_Matrix shortest_paths = get_shortest_paths(graph.function_count(), edges);


    // TODO constant
    // NOTE: Must be >=2 so that cluster_count = 1 actually grows
    const int cluster_count_growth_ratio = 2;
    size_t cluster_count = 2; // Because positioning of cluster centers is relative to each other sensible cluster_count >= 2.
    const double growing_iter_count =
        std::log( double(graph.function_count()) )
        /
        std::log(cluster_count_growth_ratio)
        + 1;    // I just want one last final iteration with every vertex being a cluster

    for(size_t i = 0; i < growing_iter_count; ++i) {

        const std::vector<V_Id> cluster_centers = get_cluster_centers(cluster_count, shortest_paths);

        // TODO check
        // TODO nedavalo by smysl radius pomalu snizovat jak se zvysuje pocet center clusteru?
        const Topo_Dist radius = std::max(
            get_max_distance_in_set(shortest_paths, cluster_centers),
            static_cast<size_t>( std::log(graph.function_count()) )    // in case there is only one cluster
        ) * gls.localNeighRadius;

        lay_out_centers(cluster_centers, positions, shortest_paths, radius, gls.nIterations);

        // TODO optimalizace - vyhodit
        if( cluster_centers.size() >= 2 ) {

            double min_D_E_centers = get_E_dist(
                positions.at( cluster_centers.at(0) ),
                positions.at( cluster_centers.at(1) )
            );

            for(const V_Id u : cluster_centers) {
                for(const V_Id v : cluster_centers) {
                    const double cand_D_E = get_E_dist( positions.at(u), positions.at(v) );
                    if( min_D_E_centers > cand_D_E ) { min_D_E_centers = cand_D_E; }
                }
            }

            // TODO pouzit min_D_E_centers jako cluster radius

            // TODO DEBUG - randomizatin is too strong?
            positions = create_random_clusters(positions, cluster_centers, shortest_paths, 0.1);
        }

        cluster_count = std::min<size_t>(
            graph.function_count(),
            cluster_count * cluster_count_growth_ratio
        );
    }

    std::unordered_map<Function_Id, position3> result;

    for(std::size_t v = 0; v < positions.size(); ++v) {
        result.emplace( vertex_to_function_id.at(v), positions.at(v) );
    }

    return result;// TODO renormalize_coordinates(result);
}
