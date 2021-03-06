#include <benchpress/benchpress.hpp>

#include <entt/EnttBenchmark.h>

#include <BaseBenchmark.h>

namespace entt_benchmark {

constexpr size_t _10M = 10'000'000L;

BENCHMARK("[1] entt     Creating 10M entities", [](benchpress::context *ctx) {
  EntityManager registry;

  ctx->reset_timer();
  for (size_t i = 0; i < ctx->num_iterations(); ++i) {
    ctx->stop_timer();
    std::vector<Entity> created_entities(_10M);

    ctx->start_timer();
    for (size_t c = 0; c < _10M; c++) {
      auto entity = registry.create();
      created_entities.emplace_back(entity);
    }
    ctx->stop_timer();

    // cleanup memory to avoid full memory
    for (auto entity : created_entities) {
      registry.destroy(entity);
    }
  }
})

BENCHMARK("[2] entt     Destroying 10M entities", [](benchpress::context *ctx) {
  EntityManager registry;

  ctx->reset_timer();
  for (size_t i = 0; i < ctx->num_iterations(); ++i) {
    ctx->stop_timer();
    std::vector<Entity> created_entities(_10M);

    for (size_t c = 0; c < _10M; c++) {
      auto entity = registry.create();
      created_entities.emplace_back(entity);
    }

    ctx->start_timer();
    for (auto entity : created_entities) {
      registry.destroy(entity);
    }
    ctx->stop_timer();
  }
})

BENCHMARK("[3] entt     Iterating over 10M entities, unpacking one component",
          [](benchpress::context *ctx) {
            EntityManager registry;

            for (size_t c = 0; c < _10M; c++) {
              auto entity = registry.create();
              registry.assign<PositionComponent>(entity);
            }

            ctx->reset_timer();
            for (size_t i = 0; i < ctx->num_iterations(); ++i) {
              for (auto entity : registry.view<PositionComponent>()) {
                DISABLE_REDUNDANT_CODE_OPT();
                auto &position = registry.get<PositionComponent>(entity);

                benchpress::escape(&entity);
                benchpress::escape(&position);
              }
            }
          })

BENCHMARK("[4] entt     Iterating over 10M entities, unpacking two components",
          [](benchpress::context *ctx) {
            EntityManager registry;

            for (size_t c = 0; c < _10M; c++) {
              auto entity = registry.create();
              registry.assign<PositionComponent>(entity);
              registry.assign<DirectionComponent>(entity);
            }

            ctx->reset_timer();
            for (size_t i = 0; i < ctx->num_iterations(); ++i) {
              for (auto entity :
                   registry.view<PositionComponent, DirectionComponent>()) {
                DISABLE_REDUNDANT_CODE_OPT();
                auto &position = registry.get<PositionComponent>(entity);
                auto &velocity = registry.get<DirectionComponent>(entity);

                benchpress::escape(&entity);
                benchpress::escape(&position);
                benchpress::escape(&velocity);
              }
            }
          })

BENCHMARK("[5] entt     Creating 10M entities at once",
          [](benchpress::context *ctx) {
            EntityManager registry;

            ctx->reset_timer();
            for (size_t i = 0; i < ctx->num_iterations(); ++i) {
              ctx->stop_timer();
              std::vector<Entity> created_entities(_10M);

              ctx->start_timer();
              registry.create(std::begin(created_entities),
                              std::end(created_entities));
              ctx->stop_timer();

              // cleanup memory to avoid full memory
              for (auto entity : created_entities) {
                registry.destroy(entity);
              }
            }
          })

BENCHMARK("entt     create destroy entity with components",
          [](benchpress::context *ctx) {
            EntityManager registry;

            ctx->reset_timer();
            for (size_t i = 0; i < ctx->num_iterations(); ++i) {
              auto entity = registry.create();

              registry.assign<PositionComponent>(entity);
              registry.assign<DirectionComponent>(entity);
              registry.assign<ComflabulationComponent>(entity);

              registry.destroy(entity);
            }
          })

class BenchmarkEntt
    : public ecs_benchmark::BaseBenchmark<EntityManager, Entity, Entity, Application,
                                          TimeDelta> {
public:
  BenchmarkEntt(const std::string &name, bool addmorecomplexsystem)
      : BaseBenchmark(name, addmorecomplexsystem,
                      {10, 25, 50, 100, 200, 400, 800, 1600, 3200, 5000, 10'000,
                       30'000, 100'000, 500'000, 1'000'000, 2'000'000,
                       5'000'000, 10'000'000, 20'000'000}) {}
  ~BenchmarkEntt() override = default;

  auto createOneEntity(EntityManager &registry) -> Entity override {
    return registry.create();
  }
  void assignPositionComponent(EntityManager &registry,
                               Entity &entity) override {
    registry.assign<PositionComponent>(entity);
  }
  void assignDirectionComponent(EntityManager &registry,
                                Entity &entity) override {
    registry.assign<DirectionComponent>(entity);
  }
  void assignComflabulationComponent(EntityManager &registry,
                                     Entity &entity) override {
    registry.assign<ComflabulationComponent>(entity);
  }

};

BenchmarkEntt enttbenchmarks("entt", false);
BenchmarkEntt enttbenchmarks_morecomplex("entt-morecomplex", true);

} // namespace entt_benchmark
