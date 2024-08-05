import SwiftUI
import Cocoa

class ViewController: NSViewControllr {
  @IBOutlet weak var imageView: UIImageView!
  
  var displayLink: CADisplayLink?
  var imageData: UnsafeMutablePointer<UInt8>?
  var width: Int
  var height: Int

  override func viewDidLoad() {
    super.viewDidLoad()

    let filePath = "/tmp/trackpadDrawerImg"

    let fileDescriptor = open (filePath, O_RDWR)
    if fileDescriptor == -1 {
      // error
      return
    }
    
    let fileSize = width * height * 4
    let imageData = mmap(nil, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, 0)
    if imageData == MAP_FAILED {
      // error
      return
    }

    self.imageData = imageData?.bindMemory(to: UInt8.self, capacity: fileSize)

    displayLink = CADisplayLink(target: self, selector: #selector(refreshImageView))
    displayLink?.add(to: .main, forMode: .default)
  }

  @objc func refreshImageView() {
    guard let imageData = self.imageData else {return}

    let colorSpace = CGColorSpaceCreateDeviceRGB()
    let bitmapInfo = CGBitmapInfo(rawValue: CGImageAlphaInfo.premultipliedLast.rawValue)
    if let provider = CGDataProvider(data: NSData(bytes: imageData, length: width * height * 4)),
      let cgImage = CGImage(width: width,
                            height: height,
                            bitsPerComponent: 8,
                            bitsPerPixel: 32,
                            bytesPerRow: width * 4,
                            space: colorSpace,
                            bitmapInfo: bitmapInfo,
                            provider: provider,
                            decode: nil,
                            shouldInterpolate: true,
                            intent: .defaultIntent) {
      imageView.image = UIImage(cgImage: cgImage)
    }
  }

  deinit {
    if let imageData = self.imageData {
      munmap(imageData, width * height * 4)
    }
  }


}