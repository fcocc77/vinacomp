# icono svg bajados en:
# https://material.io/resources/icons/?style=baseline
source utils/shell/util.sh

svgs='resources/svg'
images='resources/images'

color_a="$(jread stylesheet/palette.json 'b60')"
color_b="$(jread stylesheet/palette.json 'b80')"
color_c="$(jread stylesheet/palette.json 'base')"

size=128

mkdir $images

function export_png() {
    svg=$1
    suffix=$2
    color=$3

    filename=$(basename $svg)
    filename=${filename::-4}

    svg_temp="$images/$filename"_temp_.svg
    output="$images/$filename"_"$suffix".png

    if [ ! -f $output ]; then
        cp $svg $svg_temp

        # cambia el color 'fill'
        svg_temp
        sed -i "s|fill=\"none\"|fill=\"$color\"|g" $svg_temp
        sed -i "s|fill:none|fill:$color|g" $svg_temp
        sed -i "s|stroke:none|stroke:$color|g" $svg_temp
        #

        # cambia tamaño
        sed -i "s|height=\"24\"|height=\"$size\"|g" $svg_temp
        sed -i "s|width=\"24\"|width=\"$size\"|g" $svg_temp
        #
        #

        inkscape $svg_temp -e $output

        rm $svg_temp
    fi

}

for svg in $svgs/*; do
    export_png $svg 'a' $color_a &>/dev/null
    export_png $svg 'b' $color_b &>/dev/null
    export_png $svg 'c' $color_c &>/dev/null
    echo $svg 'Exporting ...'
done
