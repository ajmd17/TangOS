/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package tangosimageconverter;

import java.awt.Color;
import java.awt.image.BufferedImage;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author User
 */
public class ImageClamper {

    public static final ArrayList<Color> USABLE_COLORS;

    static {
        USABLE_COLORS = new ArrayList<>();
        USABLE_COLORS.add(new Color(0, 0, 0));
        USABLE_COLORS.add(new Color(0, 0, 170));
        USABLE_COLORS.add(new Color(0, 170, 0));
        USABLE_COLORS.add(new Color(0, 170, 170));
        USABLE_COLORS.add(new Color(170, 0, 0));
        USABLE_COLORS.add(new Color(170, 0, 170));
        USABLE_COLORS.add(new Color(170, 85, 0));
        USABLE_COLORS.add(new Color(170, 170, 170));
        USABLE_COLORS.add(new Color(85, 85, 85));
        USABLE_COLORS.add(new Color(85, 85, 255));
        USABLE_COLORS.add(new Color(85, 255, 85));
        USABLE_COLORS.add(new Color(85, 255, 255));
        USABLE_COLORS.add(new Color(255, 85, 85));
        USABLE_COLORS.add(new Color(255, 85, 255));
        USABLE_COLORS.add(new Color(255, 255, 85));
        USABLE_COLORS.add(new Color(255, 255, 255));
    }

    public static double colorDistance(Color c1, Color c2) {
        int red1 = c1.getRed();
        int red2 = c2.getRed();
        int rmean = (red1 + red2) >> 1;
        int r = red1 - red2;
        int g = c1.getGreen() - c2.getGreen();
        int b = c1.getBlue() - c2.getBlue();
        return Math.sqrt((((512 + rmean) * r * r) >> 8) + 4 * g * g + (((767 - rmean) * b * b) >> 8));
    }
}
