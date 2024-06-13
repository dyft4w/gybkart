public class MapInfo {
    private double[] _force_coords;
    private boolean _reset_momentum;
    private boolean _legal;
    private double _coeff;
    private int _lap_status;
    MapInfo(){
        _reset_momentum = false;
        _coeff = 1;
        _force_coords = null;
        _legal=true;
        _lap_status=0;
    }
    MapInfo(double[] force_coords, boolean reset_momentum, boolean legal, double coeff, int lap_status){
        _reset_momentum = reset_momentum;
        _coeff = coeff;
        _force_coords = force_coords;
        _legal=legal;
        _lap_status=lap_status;
    }

    public double[] getCoords() {
        return _force_coords;
    }

    public void setCoords(double[] _force_coords) {
        this._force_coords = _force_coords;
    }

    public boolean getMomentum() {
        return _reset_momentum;
    }

    public void setMomentum(boolean _reset_momentum) {
        this._reset_momentum = _reset_momentum;
    }

    public double getCoeff() {
        return _coeff;
    }

    public void setCoeff(double _coeff) {
        this._coeff = _coeff;
    }

    public boolean isLegal() {
        return _legal;
    }

    public void setLegal(boolean _legal) {
        this._legal = _legal;
    }

    public int getLapStatus() {
        return _lap_status;
    }

    public void setLapStatus(int _lap_status) {
        this._lap_status = _lap_status;
    }
}
