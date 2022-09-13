#include <JuceHeader.h>

#include "GameComponent.h"

class Application : public juce::JUCEApplication {
 public:
  Application() = default;

  const juce::String getApplicationName() override { return ProjectInfo::projectName; }
  const juce::String getApplicationVersion() override { return ProjectInfo::versionString; }
  bool moreThanOneInstanceAllowed() override { return true; }

  void initialise(const juce::String& commandLine) override {
    gameWindow.reset(new GameWindow(getApplicationName()));
  }

  void shutdown() override {
    gameWindow = nullptr;
  }

  void systemRequestedQuit() override {
    quit();
  }

  class GameWindow : public juce::DocumentWindow {
   public:
    GameWindow(juce::String name)
        : DocumentWindow(name,
                         juce::Desktop::getInstance().getDefaultLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId),
                         DocumentWindow::allButtons) {
      setUsingNativeTitleBar(true);
      const auto gameComponent = new GameComponent();
      setContentOwned(gameComponent, true);
      addKeyListener(gameComponent);
      setResizable(true, true);
      centreWithSize(getWidth(), getHeight());
      setVisible(true);
    }

    void closeButtonPressed() override {
      JUCEApplication::getInstance()->systemRequestedQuit();
    }

   private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GameWindow)
  };

 private:
  std::unique_ptr<GameWindow> gameWindow;
};

START_JUCE_APPLICATION(Application)