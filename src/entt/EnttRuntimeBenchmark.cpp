#include "EnttRuntimeBenchmark.h"

namespace enttruntime_benchmark {

MovementSystem::MovementSystem(EntityManager &registry)
    : types({registry.type<PositionComponent>(),
             registry.type<DirectionComponent>()}){};

void MovementSystem::update(EntityManager &registry, TimeDelta dt) {
  registry.runtime_view(std::begin(types), std::end(types))
      .each([&](auto entity) {
        auto &position = registry.get<PositionComponent>(entity);
        auto &direction = registry.get<DirectionComponent>(entity);

        position.x += direction.x * dt;
        position.y += direction.y * dt;
      });
}

ComflabSystem::ComflabSystem(EntityManager &registry)
    : types({registry.type<ComflabulationComponent>()}){};

void ComflabSystem::update(EntityManager &registry, TimeDelta dt) {
  registry.runtime_view(std::begin(types), std::end(types))
      .each([&](auto entity) {
        auto &comflab = registry.get<ComflabulationComponent>(entity);

        comflab.thingy *= 1.000001f;
        comflab.mingy = !comflab.mingy;
        comflab.dingy++;
        // comflab.stringy = std::to_string(comflab.dingy);
      });
}

#ifdef USE_MORECOMPLEX_SYSTEM

MoreComplexSystem::MoreComplexSystem(EntityManager &registry)
    : types({registry.type<PositionComponent>(),
             registry.type<DirectionComponent>(),
             registry.type<ComflabulationComponent>()}){};

void MoreComplexSystem::update(EntityManager &registry, TimeDelta dt) {
  registry.runtime_view(std::begin(types), std::end(types))
      .each([&](auto entity) {
        auto &position = registry.get<PositionComponent>(entity);
        auto &direction = registry.get<DirectionComponent>(entity);
        auto &comflab = registry.get<ComflabulationComponent>(entity);

        std::vector<double> vec;
        for (size_t i = 0; i < comflab.dingy && i < 100; i++) {
          vec.push_back(i * comflab.thingy);
        }

        int sum = std::accumulate(std::begin(vec), std::end(vec), 0.0);
        int product = std::accumulate(std::begin(vec), std::end(vec), 1,
                                      std::multiplies<double>());

        comflab.stringy = std::to_string(comflab.dingy);

        if (comflab.dingy % 10000 == 0) {
          if (position.x > position.y) {
            direction.x = random(0, 5);
            direction.y = random(0, 10);
          } else {
            direction.x = random(0, 10);
            direction.y = random(0, 5);
          }
        }
      });
}
};
#endif

Application::Application() {
  this->systems_.emplace_back(
      std::make_unique<MovementSystem>(this->entities_));
  this->systems_.emplace_back(std::make_unique<ComflabSystem>(this->entities_));
#ifdef USE_MORECOMPLEX_SYSTEM
  this->systems_.emplace_back(
      std::make_unique<MoreComplexSystem>(this->entities_));
#endif
}

void Application::update(TimeDelta dt) {
  for (auto &system : this->systems_) {
    system->update(this->entities_, dt);
  }
}

} // namespace enttruntime_benchmark