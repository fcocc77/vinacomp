sudo dnf -y install \
	opencv-devel \
	inkscape \
	qt5-qtbase \
	qt5-qtbase-devel \
    mesa-libGL-devel \
    mesa-libGLU-devel \
    qt5-qtmultimedia.x86_64 \
    qt5-qtmultimedia-devel.x86_64 \


pip3 install -r requirements.txt

git submodule update --init --recursive
