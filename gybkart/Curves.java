public class Curves {
    public static double forward(double t){
        return Util.clamp(0.7*Math.log((double) t /1.8)+0.2,0,7);
    }
    public static double angle(double t){
        return Util.clamp(0.0008*t+0.015, 0, 0.03);
    }
    public static double backwards(double t){
        return Util.clamp(0.3*Math.log((double) t /1.8)+0.5,0,1);
    }
}
