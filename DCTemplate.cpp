#include <vector>
#include <algorithm>
#include <queue>
#include <thread>
#include <atomic>
#include <iostream>

template<typename Tin, typename Tout>
Tout dc_seq(Tin input,
            bool (*basecase)(Tin),
            Tout (*solve)(Tin),
            std::vector<Tin> (*divide)(Tin),
            Tout (*conquer)(std::vector<Tout>)) {

    if (basecase(input)) {
        return (solve(input));
    } else {
        auto subproblems = divide(input);

        std::transform(subproblems.begin(),
                       subproblems.end(),
                       subproblems.begin(),
                       [&](Tin x) {
                           auto res = dc_seq(x, basecase, solve, divide, conquer);
                           return (res);
                       });

        auto result = conquer(subproblems);
        return (result);
    }
};

template<typename Tin, typename Tout>
Tout dc_par(Tin input,
            bool (*basecase)(Tin),
            Tout (*solve)(Tin),
            std::vector<Tin> (*divide)(Tin),
            Tout (*conquer)(std::vector<Tout>),
            int nw) {

    std::vector<Tin> tasks;
    tasks.reserve(nw * 2);
    tasks.push_back(input);
    std::atomic<int> remaining(0);


    while (tasks.size() < nw) {
        std::vector<Tin> tmp;
        tmp.reserve(nw * 2);
        for (int i = 0; i < tasks.size(); ++i) {
            std::vector<Tin> aus = divide(tasks[i]);
            tmp.insert(tmp.end(), aus.begin(), aus.end());
        }
        tmp.swap(tasks);
    }

    remaining = tasks.size();
    std::vector<Tout> outputs(tasks.size());
    std::vector<std::thread *> tid;
    tid.reserve(nw);
    auto body = [&]() {
        while (remaining >= 0) {
            int my_task = remaining.fetch_sub(1);
            my_task--;
            if (my_task >= 0) {
                outputs[my_task] = dc_seq(tasks[my_task], basecase, solve, divide, conquer);
            }
        }
    };

    for (int i = 0; i < nw; i++)
        tid.push_back(new std::thread(body));

    for (auto &t : tid)
        t->join();

    auto result = conquer(outputs);
    return (result);

};



