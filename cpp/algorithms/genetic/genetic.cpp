#include "openGA.hpp"
#include "../../utils/graph_utils.h"
using namespace std;

struct MySolution {
    Path paths[2];
};
ostream &operator<<(ostream &os, const MySolution &s) {
    return os << s.paths[0] << '\n' << s.paths[1] << endl;
}

struct MyMiddleCost {
    int cost;
};

typedef EA::Genetic<MySolution, MyMiddleCost> GA_Type;
typedef EA::GenerationType<MySolution, MyMiddleCost> Generation_Type;

namespace genetic_algorithm {
    vector <Point> points;
    eval_func evaluation;
    MySolution final_answer;
}
using namespace genetic_algorithm;

void init_genes(MySolution &p, const function<double(void)> &rnd01) {
    for(int i = 0; i < 2; i++) {
        vector <int> inds(points.size());
        iota(inds.begin(), inds.end(), 0);
        random_shuffle(inds.begin(), inds.end());
        for(int j = 0; j < inds.size(); j++)
            p.paths[i].add(points[inds[j]]);
    }
}

bool eval_solution(const MySolution &p, MyMiddleCost &c) {
    c.cost = evaluation(check_cross_free_packing_paths(p.paths[0], p.paths[1], false));
    return true; // genes are accepted
}


MySolution mutate(const MySolution &X_base, const function<double(void)> &rnd01, double shrink_scale) {
    MySolution X_new=X_base;
    int mu_count = distr(eng)%(X_base.paths[0].points.size() / 4)+1;
    // cerr << mu_count << endl;
    while(mu_count--) {
        int path_ind = (int) (distr(eng)%2);
        int point_ind = (int) (distr(eng)%(X_base.paths[0].points.size()-1));
        swap(X_new.paths[path_ind].points[point_ind], X_new.paths[path_ind].points[point_ind+1]);
    }
    return X_new;
}

MySolution swap_mutate(const MySolution &X_base, const function<double(void)> &rnd01, double shrink_scale) {
    MySolution X_new=X_base;
    int mu_count = distr(eng)%((int)sqrt(X_base.paths[0].points.size())*2)+1;
    // cerr << mu_count << endl;
    while(mu_count--) {
        int path_ind = (int) (distr(eng)%2);
        int point_ind1 = (int) (distr(eng)%(X_base.paths[0].points.size()-1));
        int point_ind2 = (int) (distr(eng)%(X_base.paths[0].points.size()-1));
        swap(X_new.paths[path_ind].points[point_ind1], X_new.paths[path_ind].points[point_ind2]);
    }
    return X_new;
}

MySolution pmx_crossover(const MySolution &X1, const MySolution &X2, const function<double(void)> &rnd01) {
    MySolution X_new;
    int len = X1.paths[0].points.size(), p1_ind = distr(eng) % 2, p2_ind = distr(eng) % 2;
    int l = distr(eng) % len, r = distr(eng) % len;
    if(l > r)
        swap(l, r);
    r++;
    int path_index=1;
    while(path_index != -1) {
        if(path_index == 0) {
            p1_ind = 1 - p1_ind;
            p2_ind = 1 - p2_ind;
        }
        const vector <Point> points1 = X1.paths[p1_ind].points;
        const vector <Point> points2 = X2.paths[p2_ind].points;
        map <Point, int> plc1, plc2;
        vector <bool> mark(len, false);
        for(int i = 0; i < len; i++) {
            plc1[points1[i]] = i;
            plc2[points2[i]] = i;
        }

        X_new.paths[path_index].points.resize(len);
        for(int i = l; i < r; i++) {
            X_new.paths[path_index].points[i] = points1[i];
            mark[i] = true;
            int ind1 = plc1[points2[i]];
            if(l <= ind1 and ind1 < r)
                continue;
            int now = i;
            while(l <= now and now < r)
                now = plc2[points1[now]];
            X_new.paths[path_index].points[now] = points2[i];
            mark[now] = true;
        }

        for(int i = 0; i < len; i++)
            if(!mark[i])
                X_new.paths[path_index].points[i] = points2[i];
        path_index--;
    }
    return X_new;
}

MySolution simple_crossover(const MySolution &X1, const MySolution &X2, const function<double(void)> &rnd01) {
    MySolution X_new;
    int p1_ind = distr(eng) % 2, p2_ind = distr(eng) % 2;
    X_new.paths[0] = X1.paths[p1_ind];
    X_new.paths[1] = X2.paths[p2_ind];
    return X_new;
}

MySolution crossover(const MySolution &X1, const MySolution &X2, const function<double(void)> &rnd01) {
    MySolution X_new;
    int len = X1.paths[0].points.size(), p1_ind = distr(eng) % 2, p2_ind = distr(eng) % 2;
    int l = distr(eng) % len, r = distr(eng) % len;
    if(l > r)
        swap(l, r);
    r++;
    int path_index=1;
    while(path_index != -1) {
        map <Point, bool> mark;
        if(path_index == 0) {
            p1_ind = 1 - p1_ind;
            p2_ind = 1 - p2_ind;
        }
        X_new.paths[path_index].points.resize(len);
        for(int i = l; i < r; i++) {
            X_new.paths[path_index].points[i] = X1.paths[p1_ind].points[i];
            mark[X1.paths[p1_ind].points[i]] = true;
        }
        int j = (l > 0 ? 0 : r);
        for(int i = 0; i < len; i) {
            while(i < len and mark[X2.paths[p2_ind].points[i]])
                i++;
            if(i < len)
                X_new.paths[path_index].points[j++] = X2.paths[p2_ind].points[i++];
            if(j == l)
                j = r;
        }
        path_index--;
    }
    return X_new;
}

double calculate_SO_total_fitness(const GA_Type::thisChromosomeType &X) {
    return X.middle_costs.cost;
}

void SO_report_generation(int generation_number, const EA::GenerationType<MySolution, MyMiddleCost> &last_generation,
                          const MySolution &best_genes) {
    final_answer = best_genes;
}

pair <Path, Path> run_genetic(vector <Point> input_points, eval_func input_evaluation) {
    points = input_points;
    evaluation = input_evaluation;

    EA::Chronometer timer;
    timer.tic();

    GA_Type ga_obj;
    ga_obj.problem_mode = EA::GA_MODE::SOGA;
    ga_obj.multi_threading = true;
    ga_obj.dynamic_threading = true;
    ga_obj.idle_delay_us = 1; // switch between threads quickly
    ga_obj.verbose = false;
    ga_obj.verbose2 = false;
    ga_obj.population = 300;
    ga_obj.generation_max = 500;
    ga_obj.calculate_SO_total_fitness = calculate_SO_total_fitness;
    ga_obj.init_genes = init_genes;
    ga_obj.eval_solution = eval_solution;
    ga_obj.mutate = swap_mutate;
    ga_obj.crossover = crossover;
    ga_obj.SO_report_generation = SO_report_generation;
    ga_obj.best_stall_max = 200;
    ga_obj.average_stall_max = 1000000;
    ga_obj.tol_stall_best = 0;
    ga_obj.elite_count = 10;
    ga_obj.crossover_fraction = 0.6;
    ga_obj.mutation_rate = 0.4;
    ga_obj.solve();

    cerr << "The problem is optimized in " << timer.toc() << " seconds." << endl;

    return {final_answer.paths[0], final_answer.paths[1]};
}