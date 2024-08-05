import Cocoa



func main() {
  let app = NSApplication.shared
  app.setActivationPolicy(.regular)
  app.activate(ignoringOtherApps: true);

  let window = NSWindow(
    contentRect: NSRect(x: 0, y: 0, width: 800, height: 600), // placeholder width and height
    styleMask: [.titled, .closable, .resizable, .miniaturizable],
    backing: .buffered,
    defer: false
  )
  window.center()

  let mainViewController = MainViewController()
  window.contentViewController = mainViewController

  app.run();
}

main()
