import greenfoot.Greenfoot;

import java.awt.*;
import java.awt.geom.AffineTransform;
import java.awt.image.BufferedImage;

public class Util {
    public static double clamp(double value, double min, double max){
        return Math.max(Math.min(value,max),min);
    }
    // https://stackoverflow.com/questions/37758061/rotate-a-buffered-image-in-java
    public static BufferedImage rotateImageByDegrees(BufferedImage img, double rads, int cent_x, int cent_y) {
        int w = img.getWidth();
        int h = img.getHeight();
        int new_width = w/2;
        int new_height = h/2;

        BufferedImage rotated = new BufferedImage(new_width, new_height, BufferedImage.TYPE_INT_ARGB);
        Graphics2D g2d = rotated.createGraphics();
        AffineTransform at = new AffineTransform();
        at.translate((double) (new_width - w) / 2, (double) (new_height - h) / 2);

        at.rotate(rads, cent_x, cent_y);
        g2d.setTransform(at);
        g2d.drawImage(img, 0, 0, null);
        g2d.dispose();

        return rotated;
    }
    public static double[] calcVec(double deg){
        return new double[] {Math.sin(deg), Math.cos(deg)};
    }
    public static int getHexCode(greenfoot.Color color){
        int temp = 0;
        temp+=(color.getRed()<< 16);
        temp+=(color.getGreen() << 8);
        temp+=color.getBlue();
        return temp;
    }
    public static String formatTime(long ms_time){
        return String.format("%02d:%02d:%03d", (int)(Math.floor((double) ms_time /60000) % 60), (int)(Math.floor((double) ms_time /1000) % 60), ms_time % 1000);
    }
    public static BufferedImage scale(BufferedImage img, int s){
        int w = img.getWidth();
        int h = img.getHeight();
        Graphics2D g2d = (Graphics2D) img.getGraphics();
        g2d.setRenderingHint(RenderingHints.KEY_INTERPOLATION,
                RenderingHints.VALUE_INTERPOLATION_BILINEAR);
        BufferedImage new_img= new BufferedImage(w*s,h*s,BufferedImage.TYPE_INT_ARGB);
        g2d.drawImage(img, 0, 0, w*s, h*s, 0, 0, w, h, null);
        return new_img;
    }
}
