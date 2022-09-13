#pragma once

#include <JuceHeader.h>
#include "Game.hpp"

class GameComponent : public juce::Component, public juce::KeyListener {
 public:
  GameComponent();
  virtual ~GameComponent() = default;

  void paint(juce::Graphics& g) override;
  bool keyPressed(const juce::KeyPress& key, juce::Component*) override;

 private:
  Game engine;

  void _drawTile(juce::Graphics& g, const juce::Rectangle<int>& rect, int value, int x, int y);
  int _offsetCoord(int coord, int size, int offset);

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GameComponent)
};
