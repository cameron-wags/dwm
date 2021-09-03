pkgname=dwm
pkgver=6.2_p2.1
pkgrel=1
epoch=
pkgdesc="my fork of dwm"
arch=("any")
url="https://github.com/cameron-wags/dwm"
license=("MIT")
depends=()
source=("$pkgname-$pkgver.tar.gz::$url/archive/refs/tags/$pkgver.tar.gz")
md5sums=('61f94278f7671bbf0952da27d78078ae')

package() {
    cd "$pkgname-$pkgver"
    make DESTDIR="$pkgdir" install
}
