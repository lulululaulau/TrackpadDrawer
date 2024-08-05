import Cocoa

class MainViewController: NSViewController {
  override func loadView() {
    self.view = NSView(frame: NSRect(x: 0, y: 0, width: 800, height: 600))
    self.view.wantsLayer = true
    self.view.layer?.backgroundColor = NSColor.white.cgColor

    let stackView = NSStackView()
    stackView.orientation = .vertical
    stackView.alignment = .leading
    stackView.distribution = .fillProportionally
    stackView.spacing = 0

    let toolbar = Toolbar()
    addChild(toolbar)
    stackView.addArrangedSubview(toolbar.view)
    toolbar.view.translatesAutoresizingMaskIntoConstrants = false

    let imageDisplay = ImageDisplay()
    addChild(imageDisplay)
    stackView.addArrangedSubview(imageDisplay)
    imageDisplay.view.translatesAutoresizingMaskIntoConstrants = false

    stackView.translatesAutoresizingMaskIntoConstrants = false
    self.view.addSubview(stackView)

    NSLayoutConstraint.activate([
      stackView.topAnchor.constraint(equalTo: self.view.topAnchor, constant: 20),
      stackView.leadingAnchor.constraint(equalTo: self.view.leadingAnchor, constant: 20),
      stackView.trailingAnchor.constraint(equalTo: self.view.trailingAnchor, constant: -20),
      stackView.bottomAnchor.constraint(equalTo: self.view.bottomAnchor, constant: -20)
    ])

  }

  override func viewDidLoad() {
    super.viewDidLoad()
  }

}