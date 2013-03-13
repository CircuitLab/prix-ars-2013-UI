//
//  ArsUIUtil.h
//  arsUI
//
//  Created by hideyukisaito on 2013/03/12.
//
//

#ifndef arsUI_ArsUIUtil_h
#define arsUI_ArsUIUtil_h

#define FUJI_LAT 35.362841
#define FUJI_LON 138.730674

#define YAMANAKA_LAKE_LAT 35.407591
#define YAMANAKA_LAKE_LON 138.876039

class ArsUIUtil
{
    public:
        static float distance(ofPoint p1, ofPoint p2) {
            float a = p1.x - p2.x;
            float b = p1.y - p1.y;
            return abs(sqrt(a * a + b * b));
        };
    
        static ofPoint GPStoXY(ofPoint fujiPos, double lat, double lon) {
            ofPoint yamanakaLakePos = ofPoint(1263, 489);
            
            ofPoint p;
            p.x = fujiPos.x + (lon - FUJI_LON) * ((yamanakaLakePos.x - fujiPos.x) / (YAMANAKA_LAKE_LON - FUJI_LON));
            p.y = fujiPos.y + (lat - FUJI_LAT) * ((yamanakaLakePos.y - fujiPos.y) / (YAMANAKA_LAKE_LAT - FUJI_LAT));
            return p;
//            double fujiLat = 35.362841;
//            double fujiLon = 138.730674;
//            
//            //yamanakako bench (旭日丘 交差点)
//            double yamanakaLat = 35.407591;
//            double yamanakaLon = 138.876039;
//            ofPoint yamanaka = ofPoint(1263, 489);
//            
//            ofPoint newPoint;
//            
//            newPoint.x = fuji.x + (_lon -fujiLon) * ((yamanaka.x - fuji.x)/ (yamanakaLon - fujiLon));
//            newPoint.y = fuji.y + (_lat -fujiLat) * ((yamanaka.y - fuji.y)/ (yamanakaLat - fujiLat));
//            //cout << "x" << newPoint.x << " " <<  newPoint.y;
//            return newPoint;
        };
    
    static ofPoint XYtoGPS(ofPoint fujiPos, ofPoint pos) {
        
    };
};

#endif
