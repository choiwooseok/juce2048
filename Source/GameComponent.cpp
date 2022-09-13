#include "GameComponent.h"

constexpr int TILE_SIZE = 64;
constexpr int TILE_MARGIN = 16;

GameComponent::GameComponent()
    : engine(juce::KeyPress::escapeKey,
             juce::KeyPress::leftKey,
             juce::KeyPress::rightKey,
             juce::KeyPress::upKey,
             juce::KeyPress::downKey) {
  setSize(340, 400);
}

void GameComponent::paint(juce::Graphics& g) {
  g.fillAll(juce::Colour(COLOR_BOARD));

  const auto rect = getLocalBounds();

  for (int y = 0; y < LINE_SIZE; ++y) {
    for (int x = 0; x < LINE_SIZE; ++x) {
      _drawTile(g, rect, engine.Board(x + y * LINE_SIZE), x, y);
    }
  }

  const int width = rect.getWidth(), height = rect.getHeight();
  if (engine.IsEnd()) {
    const juce::String endMsg = engine.GetGameOverMsg();
    const auto font = juce::Font(32, juce::Font::bold);
    g.setFont(font);
    const int w = font.getStringWidth(endMsg);
    g.drawSingleLineText(endMsg, width / 2 - w / 2, height / 2);
  }

  g.setColour(juce::Colour(COLOR_TEXT));
  const auto font = juce::Font(22);
  g.setFont(font);
  const juce::String strScore = juce::String("Score: ") + juce::String(engine.Score());
  const int w = font.getStringWidth(strScore);
  g.drawSingleLineText("ESC to Restart!", TILE_MARGIN, height - 20);
  g.drawSingleLineText(strScore, width - w - TILE_MARGIN, height - 20);
}

bool GameComponent::keyPressed(const juce::KeyPress& key, juce::Component* originatingComponent) {
  if (key.isValid()) {
    engine.KeyPressed(key.getKeyCode());
    repaint();
  }
  return true;
}

void GameComponent::_drawTile(juce::Graphics& g, const juce::Rectangle<int>& rect, int value, int x, int y) {
  const int xOffset = _offsetCoord(x, rect.getWidth(), 0);
  const int yOffset = _offsetCoord(y, rect.getHeight(), TILE_MARGIN * 2);
  g.setColour(juce::Colour(engine.Background(value)));

  g.fillRoundedRectangle(xOffset, yOffset, TILE_SIZE, TILE_SIZE, 10);

  if (value) {
    const int size = (value < 100) ? 32 : (value < 1000) ? 28
                                                         : 22;
    const auto font = juce::Font(size, juce::Font::bold);
    const auto strValue = juce::String(value);
    g.setColour(juce::Colour(engine.Foreground(value)));
    g.setFont(font);
    const int w = font.getStringWidth(strValue);
    const int h = (value < 100) ? size - 6 : (value < 1000) ? size - 4
                                                            : size - 5;
    g.drawSingleLineText(strValue, xOffset + (TILE_SIZE - w) / 2,
                         yOffset + TILE_SIZE - (TILE_SIZE - h) / 2 - 2);
  }
}

int GameComponent::_offsetCoord(int coord, int size, int offset) {
  const int start = (size / 2) - (((TILE_SIZE * LINE_SIZE) + (TILE_MARGIN * (LINE_SIZE - 1))) / 2);
  return coord * (TILE_MARGIN + TILE_SIZE) + start - offset;
}