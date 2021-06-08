pkgname=dwm
pkgver=6.2_p1.0
pkgrel=1
epoch=
pkgdesc="my fork of dwm"
arch=("any")
url="https://github.com/cameron-wags/dwm"
license=("MIT")
depends=()
source=("$pkgname-$pkgver.tar.gz::$url/archive/refs/tags/$pkgver.tar.gz")
md5sums=("SKIP")

package() {
    cd "$pkgname-$pkgver"
    make DESTDIR="$pkgdir" install
}
