pkgname=dwm
pkgver=6.2_p2.0
pkgrel=1
epoch=
pkgdesc="my fork of dwm"
arch=("any")
url="https://github.com/cameron-wags/dwm"
license=("MIT")
depends=()
source=("$pkgname-$pkgver.tar.gz::$url/archive/refs/tags/$pkgver.tar.gz")
md5sums=('41c5f35fad0185d6c34dcec737d5d3c8')

package() {
    cd "$pkgname-$pkgver"
    make DESTDIR="$pkgdir" install
}
