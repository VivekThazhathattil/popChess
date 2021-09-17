
# popChess

<p float="left">
<img src="img/progress1.png" width="400" height="400"/>
</p>

## About the project

This is a simple application that lets the user view the current top rated game from Lichess. As of now, even though it is in its nascent stage, it is able to show live Lichess TV with ease. But I intend to extent the features of this app to include live broadcast (like the upcoming World Chess Championship), show live display bar giving a qualitative estimation of the evaluation of the given position, and flexibility to customize the board to tune it to your liking.

## Built With

* ![libcurl](https://curl.se/libcurl/) is used in conjuction with ![Lichess API](https://lichess.org/api) to stream the games.
* GUI is provided by ![GTK3+](https://docs.gtk.org/gtk3/) library.
* In order to parse JSON in C, I've use Neil Henning's ![jsoh.h](https://github.com/sheredom/json.h).
* To overcome the hassle of displaying SVG images using GTK, I've also made use of ![rsvg](https://cran.r-project.org/web/packages/rsvg/index.html) library.

### Getting Started

I've only tested this app on my Linux system (ArchLinux Latest version). I believe that as long as you have the following prerequisites, you can get this app working on your system.

### Prerequisites
* `libcurl` (I've tested with `libcurl 7.78.0`; The version number can be obtained by `curl-config --version`)
* `GTK3+` (I've tested with `GTK 3.24.30`; The version number may be obtained using `gtk-launch --version`)
* `librsvg`
* `GNU make` (I've used the version `make 4.3`)

### Installation
* Clone the repo with `git clone https://github.com/VivekThazhathattil/popChess`
* Navigate to the popChess directory `cd popChess`
* Run Make `make build`
* To run popChess, run Make `make run` or simply by `./popChess` (Make sure that you assign proper privileges to execute the file `popchess`; This can be done by `chmod +x popChess`)

## Contributing

Contributions are what make the open source community such an amazing place to be learn, inspire, and create. Any contributions you make are extremely appreciated.

* Fork the Project
* Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
* Commit your Changes (`git commit -m 'Add some AmazingFeaturea'`)
* Push to the Branch (`git push origin feature/AmazingFeature`)
* Open a Pull Request

## LICENSE 

Distributed under GNU General Public License. See `LICENSE` for more information.

## Contact

For any queries, please contact me, Vivek T @ [vivek.thazhathattil@gmail.com](mailto:vivek.thazhathattil@gmail.com)

Project link: [https://github.com/VivekThazhathattil/popChess](https://github.com/VivekThazhathattil/popChess)

## Acknowledgements

* [Lichess](https://lichess.org/)
* [GTK3+](https://docs.gtk.org/gtk3/)
* Neil Henning [sheredom](https://github.com/sheredom) for [jsoh.h](https://github.com/sheredom/json.h)


https://user-images.githubusercontent.com/63693789/133836525-5c7d5bfc-bf0e-4e2e-afab-0f424938db73.mp4

