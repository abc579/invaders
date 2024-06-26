#pragma once

#include "invaders_math.h"
#include "event.h"

#include <cmath>
#include <vector>

#define CELL_WIDTH  220.0f
#define CELL_HEIGHT 160.0f

namespace Game {

  enum class EntityType{ ALIEN, PLAYER };

  class EntityGridData final {
  public:
    void* data;
    EntityType type;
  };

  class GridManager final {
  public:
    GridManager(const int sceneWidth,
		const int sceneHeight,
		Ev::EventManager& eventManager);
    ~GridManager();
    void beginFrame();
    void update(const Math::v3& pos, void* ent, const EntityType type);
    std::vector<EntityGridData> getNearby(const Math::v3& pos, const EntityType wanted);
  private:
    inline auto getId(const Math::v3& pos) const noexcept
    {
      return std::floor(pos.x / pixelsToWorld(CELL_WIDTH)) + std::floor(pos.y / pixelsToWorld(CELL_HEIGHT)) * m_cols;
    }
    std::vector<std::vector<EntityGridData>> m_grid;
    Ev::EventManager& m_eventManager;
    int m_cols;
    int m_rows;
  };

};
