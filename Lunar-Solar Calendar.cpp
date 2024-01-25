#include <iostream>
#include <fstream> // work with file
#include <string>
#include <vector>
#include <chrono> // work with time
#include <ctime>
#include<cmath>
using namespace std;
string a[12] = {"January", "February", "March", "April", "May", "June", "July",
                "August", "September", "October", "November", "December"};
// Cấu trúc cho một sự kiện ghi nhớ
struct Event{
    string startTime;
    string content;
};
// Cấu trúc cho một ngày với danh sách các sự kiện
struct Day{
    string date;
    vector<Event> events;
};
vector<Day> calendar; // Lưu trữ các sự kiện cần note

// Mã màu ANSI cho màu đỏ
const string RED_COLOR = "\x1B[31m";
const string RESET_COLOR = "\x1B[0m";
// Hàm để kiểm tra năm nhuận
bool isLeapYear(int year){
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}
// Hàm để lấy số ngày trong một tháng cụ thể
int getDaysInMonth(int month, int year){
    if (month == 2){
        return isLeapYear(year) ? 29 : 28;
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11){
        return 30;
    }
    else{
        return 31;
    }
}
void print_calendar(int month, int year){
    int daysInMonth = getDaysInMonth(month, year);
    cout <<"--------------------------------------";
    cout << "\n   " << a[month - 1] << ",   " << year << "\n";
    cout <<"--------------------------------------\n";
    cout << " S  M  T  W  T  F  S\n";
    // Tính ngày đầu tiên của tháng (0: Chủ Nhật, 1: Thứ 2, ..., 6: Thứ 7)
    int startDay = (1 + (month <= 2 ? year - 1 : year) + (year - 1) / 4 - (year - 1) / 100 + (year - 1) / 400) % 7;
    // In ra khoảng trắng cho ngày đầu tiên
    for (int i = 0; i < startDay; ++i){
        cout << "   ";
    }
    // In ra các ngày trong tháng
    for (int day = 1; day <= daysInMonth; ++day){
        if ((startDay + day - 1) % 7 == 0){
            // In ngày Chủ Nhật với màu đỏ
            std::cout << RED_COLOR << (day < 10 ? " " : "") << day << " " << RESET_COLOR;
        }
        else{
            std::cout << (day < 10 ? " " : "") << day << " ";
        }
        if ((startDay + day) % 7 == 0){
            std::cout << "\n";
        }
    }
    cout << endl;
}
void getCurrentDate(int &year, int &month, int &day){
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    std::tm localTime = *std::localtime(&currentTime);

    year = localTime.tm_year + 1900;
    month = localTime.tm_mon + 1;
    day = localTime.tm_mday;
}
// Xử lý với sự kiện
void readCalendarData(std::vector<Day> &calendar){
    std::ifstream inputFile("Note_event.txt");
    if (inputFile.is_open())
    {
        calendar.clear();
        Day currentDay;
        while (inputFile >> currentDay.date)
        {
            Event event;
            std::string startTime;
            std::getline(inputFile >> std::ws, startTime, '-');
            std::getline(inputFile >> std::ws, event.content);
            event.startTime = startTime;
            
            // char nextChar;
            // inputFile.get(nextChar); // Đọc ký tự xuống dòng
            // if (nextChar == '\n') {
            //     calendar.push_back(currentDay);
            //     currentDay.events.clear();
            // }
            bool found = false;
            for (Day &i : calendar)
            {
                if (i.date == currentDay.date)
                {
                    found = true;
                    i.events.push_back(event);
                    break;
                }
            }
            if (!found)
            {   currentDay.events.push_back(event);
                calendar.push_back(currentDay);
                currentDay.events.clear();
            }
        }
        inputFile.close();
    }
}
void input_note(vector<Day>& calendar, string date){
    // Nhập thông tin sự kiện ghi nhớ
    string startTime, content;
    cout << "Nhap gio bat dau (hh:mm): ";
    cin >> startTime;
    cin.ignore(); // Xóa bộ đệm
    cout << "Nhap noi dung: ";
    getline(cin, content);
    // Tìm ngày trong danh sách calendar
    bool found = false;
    for (Day &day : calendar)
    {
        if (day.date == date)
        {
            found = true;
            Event event = {startTime, content};
            day.events.push_back(event);
            break;
        }
    }
    // Nếu ngày chưa có trong danh sách, thêm ngày mới
    if (!found)
    {
        Event event = {startTime, content};
        Day newDay = {date, {event}};
        calendar.push_back(newDay);
    }
}
void writeCalendarData(const std::vector<Day> &calendar){
    std::ofstream outputFile("Note_event.txt");
    if (outputFile.is_open())
    {
        for (const Day &day : calendar)
        {
            for (const Event &event : day.events)
            {
                outputFile << day.date << "\n";
                outputFile << event.startTime << "- " << event.content << "\n";
            }
        }
        outputFile.close();
    }
    else
    {
        std::cerr << "Không thể mở file.\n";
    }
}//20/11/2023
bool check_1year(string sdate){
    int day = stoi(sdate.substr(0, 2));
    int month = stoi(sdate.substr(3, 5));
    int year = stoi(sdate.substr(6, 10));
    time_t now = time(nullptr);
    tm localTime = *std::localtime(&now);

    int currentYear = localTime.tm_year + 1900;
    int currentMonth = localTime.tm_mon + 1;
    int currentDay = localTime.tm_mday;

    int maxYear = currentYear + 1;
    int minYear = currentYear - 1;

    if (year > maxYear || year < minYear)
        return false;
    if (year == minYear && (month < currentMonth || (month == currentMonth && day < currentDay)))
        return false;
    if (year == maxYear && (month > currentMonth || (month == currentMonth && day > currentDay)))
        return false;
    return true;
}
void delete_event(vector<Day> &calendar){
    for (auto it = calendar.begin(); it != calendar.end(); ++it)
    {
        if (!check_1year(it->date))
        {
            calendar.erase(it);
            --it;
        }
    }
}
bool check_3year(string sdate){
    int day = stoi(sdate.substr(0, 2));
    int month = stoi(sdate.substr(3, 5));
    int year = stoi(sdate.substr(6, 10));
    time_t now = time(nullptr);
    tm localTime = *std::localtime(&now);

    int currentYear = localTime.tm_year + 1900;
    int currentMonth = localTime.tm_mon + 1;
    int currentDay = localTime.tm_mday;
    int maxYear = currentYear + 3;
    if (year > maxYear)
        return false;
    if (year < maxYear)
        return true;
    if (year == maxYear && (month > currentMonth || (month == currentMonth && day > currentDay)))
        return false;
    return true;
}
// Phần dành cho lịch âm
const double PI = 3.14159265358979323846;
const double a_Lunarday = 29.53058868;// 1 ngày âm lịch = 29,5 ngày bình thường
double LOCAL_TIMEZONE = 7.0;// gio dia phuong VN
  // Sự kiện ngày 15/10/1582 có sự thay đổi cách tính lịch
double UniversalToJD(int day, int month, int year){ // Đổi ngày dương lịch ra số ngày Julius
    double JD;
	if (year > 1582 || (year == 1582 && month > 10) || (year == 1582 && month == 10 && day > 14)) {
		JD = 367*year - int(7*(year+int((month+9)/12))/4) - int(3*(int((year+(month-9)/7)/100)+1)/4) + int(275*month/9)+day+1721028.5;
	} else {
		JD = 367*year - int(7*(year+5001+int((month-9)/7))/4) + int(275*month/9)+day+1729776.5;
	}
	return JD;
}
vector<int> UniversalFromJD(double JD) {
	int Z, A, alpha, B, C, D, E, dd, mm, yyyy;
	double F;
	Z = int(JD+0.5);
	F = (JD+0.5)-Z;
	if (Z < 2299161) {
	  A = Z;
	} else {
	  alpha = int((Z-1867216.25)/36524.25);
	  A = Z + 1 + alpha - int(alpha/4);
	}
	B = A + 1524;
	C = int( (B-122.1)/365.25);
	D = int( 365.25*C );
	E = int( (B-D)/30.6001 );
	dd = int(B - D - int(30.6001*E) + F);
	if (E < 14) {
	  mm = E - 1;
	} else {
	  mm = E - 13;
	}
	if (mm < 3) {
	  yyyy = C - 4715;
	} else {
	  yyyy = C - 4716;
	}
	return vector<int> {dd,mm, yyyy};
}
double NewMoon(int k){ // Tính thời điểm Sóc
    double T = k/1236.85; // Time in Julian centuries from 1900 January 0.5
	double T2 = T * T;
	double T3 = T2 * T;
	double dr = PI/180; // PI is PI = 3.1415... dr = chuyển từ độ sang PI
	double Jd1 = 2415020.75933 + 29.53058868*k + 0.0001178*T2 - 0.000000155*T3;
	Jd1 = Jd1 + 0.00033*sin((166.56 + 132.87*T - 0.009173*T2)*dr); // Mean new moon
	double M = 359.2242 + 29.10535608*k - 0.0000333*T2 - 0.00000347*T3; // Sun's mean anomaly
	double Mpr = 306.0253 + 385.81691806*k + 0.0107306*T2 + 0.00001236*T3; // Moon's mean anomaly
	double F = 21.2964 + 390.67050646*k - 0.0016528*T2 - 0.00000239*T3; // Moon's argument of latitude
	double C1=(0.1734 - 0.000393*T)*sin(M*dr) + 0.0021*sin(2*dr*M);
	C1 = C1 - 0.4068*sin(Mpr*dr) + 0.0161*sin(dr*2*Mpr);
	C1 = C1 - 0.0004*sin(dr*3*Mpr);
	C1 = C1 + 0.0104*sin(dr*2*F) - 0.0051*sin(dr*(M+Mpr));
	C1 = C1 - 0.0074*sin(dr*(M-Mpr)) + 0.0004*sin(dr*(2*F+M));
	C1 = C1 - 0.0004*sin(dr*(2*F-M)) - 0.0006*sin(dr*(2*F+Mpr));
	C1 = C1 + 0.0010*sin(dr*(2*F-Mpr)) + 0.0005*sin(dr*(2*Mpr+M));
	double deltat;
	if (T < -11) {
		deltat= 0.001 + 0.000839*T + 0.0002261*T2 - 0.00000845*T3 - 0.000000081*T*T3;
	} else {
		deltat= -0.000278 + 0.000265*T + 0.000262*T2;
	};
	double JdNew = Jd1 + C1 - deltat;
	return JdNew;
}
vector<int> LocalFromJD(double JD) { // Chuyển đổi số ngày Julius / ngày dương lịch theo giờ địa phương
		return UniversalFromJD(JD + LOCAL_TIMEZONE/24.0);
}
double LocalToJD(int D, int M, int Y) { // Chuyển đổi ngày dương lịch theo giờ địa phương/ số ngày Julius
		return UniversalToJD(D, M, Y) - LOCAL_TIMEZONE/24.0;
}
double SUNLONG_MAJOR[] =  { // Các thời điểm xuất hiện Trung Khí thì mặt trời có các tọa độ sau
	0, PI/6, 2*PI/6, 3*PI/6, 4*PI/6, 5*PI/6, PI, 7*PI/6, 8*PI/6, 9*PI/6, 10*PI/6, 11*PI/6
};
double SunLongitude(double jdn) { // Tính vị trí mặt trời tại 1 thời điểm thông qua số ngày jdn (julius)
	double T = (jdn - 2451545.0 ) / 36525; // Time in Julian centuries from 2000-01-01 12:00:00 GMT
	double T2 = T*T;
	double dr = PI/180; // degree to radian
	double M = 357.52910 + 35999.05030*T - 0.0001559*T2 - 0.00000048*T*T2; // mean anomaly, degree
	double L0 = 280.46645 + 36000.76983*T + 0.0003032*T2; // mean longitude, degree
	double DL = (1.914600 - 0.004817*T - 0.000014*T2)*sin(dr*M);
	DL = DL + (0.019993 - 0.000101*T)*sin(dr*2*M) + 0.000290*sin(dr*3*M);
	double L = L0 + DL; // true longitude, degree
	L = L*dr;
	L = L - PI*2*(int(L/(PI*2))); // Trả về giá trị góc trong khoảng (0, 2*PI)
	return L;
}
vector<int> LunarMonth11(int Y) { // Tính tháng âm lịch chứa ngày đông chí
	double off = LocalToJD(31, 12, Y) - 2415021.076998695;
	int k = int(off / 29.530588853);
	double jd = NewMoon(k);
	vector<int> ret = LocalFromJD(jd);
	double sunLong = SunLongitude(LocalToJD(ret[0], ret[1], ret[2])); // sun longitude at local midnight
	if (sunLong > 3*PI/2) { // Đông chí là thời điểm mà kinh độ của mặt trời trên đường hoàng đạo là 3*PI/2
		jd = NewMoon(k-1);
	}
	return LocalFromJD(jd);
}
void initLeapYear(vector<vector<int>>& ret) { // Làm việc với năm nhuận
	vector<double> sunLongitudes(ret.size());
	for (int i = 0; i < ret.size(); i++) {
		vector<int> a = ret[i];
		double jdAtMonthBegin = LocalToJD(a[0], a[1], a[2]);
		sunLongitudes[i] = SunLongitude(jdAtMonthBegin);
	}
	bool found = false;
	for (int i = 0; i < ret.size(); i++) {
		if (found) {
			ret[i][3] = (i+10)% 12;
			continue;
		}
		double sl1 = sunLongitudes[i];
		double sl2 = sunLongitudes[i+1];
        bool hasMajorTerm = std::floor(sl1/PI*6) != std::floor(sl2/PI*6);
		if (!hasMajorTerm) {
			found = true;
			ret[i][4] = 1;
			ret[i][3] = (i+10)% 12;
		}
	}		
}
vector<vector<int>> LunarYear(int Y) {
    vector<vector<int>> ret;
	vector<int> month11A = LunarMonth11(Y-1);
	double jdMonth11A = LocalToJD(month11A[0], month11A[1], month11A[2]);
	int k = std::floor(0.5 + (jdMonth11A - 2415021.076998695) / 29.530588853);
	vector<int> month11B = LunarMonth11(Y);
	double off = LocalToJD(month11B[0], month11B[1], month11B[2]) - jdMonth11A;
	bool leap = off > 365.0;
	if (!leap) {
		ret = vector<vector<int>>(13, vector<int>(5,0));
	} else {
		ret = vector<vector<int>>(13, vector<int>(5,0));
	}
	ret[0] = {month11A[0], month11A[1], month11A[2], 0, 0};
	ret[ret.size() - 1] = {month11B[0], month11B[1], month11B[2], 0, 0};
	for (int i = 1; i < ret.size() - 1; i++) {
		double nm = NewMoon(k+i);
		vector<int> a = LocalFromJD(nm);
		ret[i] = {a[0], a[1], a[2], 0, 0};
	}
	for (int i = 0; i < ret.size(); i++) {
		ret[i][3] = (i + 11) % 12;
	}
	if (leap) {
		initLeapYear(ret);
	}
	return ret;
}
void Solar2Lunar(int D, int M, int Y) { // Chuyển ngày dương sang ngày âm 
	int yy = Y;
	vector<vector<int>> ly = LunarYear(Y); // Please cache the result of this computation for later use!!!
	vector<int> month11 = ly[ly.size() - 1];
	double jdToday = LocalToJD(D, M, Y);
	double jdMonth11 = LocalToJD(month11[0], month11[1], month11[2]);
	if (jdToday >= jdMonth11) {
		ly = LunarYear(Y+1);
		yy = Y + 1;
	}
	int i = ly.size() - 1;
	while (jdToday < LocalToJD(ly[i][0], ly[i][1], ly[i][2])) {
		i--;
	}
	int dd = (int)(jdToday - LocalToJD(ly[i][0], ly[i][1], ly[i][2])) + 1;
	int mm = ly[i][3];
	if (mm >= 11) {
		yy--;
	}
    if(mm == 0){
        mm = 12;
        --yy;
    }
    cout << "--> NGAY AM LICH TUONG UNG LA: "<< dd << "/" << mm << "/"<< yy << endl;; // In ra ngày âm lịch tương ứng
}
int main(){
    int year, month, day;// Lưu giữ thông tin ngày hiện hành
    getCurrentDate(year, month, day);
    int seeing_month, seeing_year; // lưu tháng, năm mà bạn 'đang xem' lịch
    seeing_month = month;
    seeing_year = year;
    print_calendar(month, year); // Mặc định luôn in ra lịch của ngày hiện hành
    while (true){
        cout << "0. Chon thang, nam muon xem lich" << endl;
        cout << "1. An so 1 de xem lich thang truoc" << endl;
        cout << "2. An so 2 de xem lich thang sau" << endl;
        cout << "3. Nhap thong tin ghi nho: " << endl;
        cout << "4. In ra your noted list " << endl;
        cout << "5. Chuyen ngay Duong --> Am" << endl;
        cout << "6. Thoat" << endl;
        cout << "---->Your choice: ";
        int k;
        cin >> k;
        int watch_month, watch_year;
        if (k == 0){
            cout << "Nhap vao thang muon xem: ";
            cin >> watch_month;
            cout << "Nhap vao nam muon xem: ";
            cin >> watch_year;
            print_calendar(watch_month, watch_year);
        }
        if (k == 1){
            if (seeing_month == 1){
                seeing_month = 13;
                --seeing_year;
            }
            print_calendar(--seeing_month, seeing_year); // xem lịch tháng trước
        }
        if (k == 2){
            if (seeing_month == 12){
                seeing_month = 0;
                ++seeing_year; // xem lịch tháng sau
            }
            print_calendar(++seeing_month, seeing_year);
        }
        if (k == 3){
            readCalendarData(calendar);
            //    Nhập thông tin sự kiện ghi nhớ
            string date, startTime, content;
            cout << "Nhap ngay (dd/mm/yyyy): ";
            cin >> date;
            if (!check_3year(date)){ 
                // Check xem ngày nhập vào có quá 3 năm so với ngày hiện hành ko
                cout << "!!!OVER 3 YEARS FROM TODAY!!! \n";
            }
            else{
                input_note(calendar, date); // Nhập thông tin ngày ghi nhớ và sự kiện cần ghi nhớ
                // Ghi thông tin vào file
                writeCalendarData(calendar); 
            }
        }
        if (k == 4){
            readCalendarData(calendar);
            delete_event(calendar); // Xóa các sự kiện ngoài phạm vi 1 năm
            cout << "----YOUR NOTED AROUND 1 YEAR----: " << endl;
            for (auto i : calendar){
                cout << i.date << endl; // In ra ngày
                for (auto j : i.events){ // Các event ứng với mỗi ngày
                    cout << "    + " << j.startTime << " - " << j.content << endl;
                }
            }
            writeCalendarData(calendar);
        }
        if (k == 5){
            string Solar_day;
            cout << "Nhap vao ngay thang nam muon xem lich (dd/mm/yyyy): ";
            cin >> Solar_day;
            int D = stoi(Solar_day.substr(0, 2));
            int M = stoi(Solar_day.substr(3, 5));
            int Y = stoi(Solar_day.substr(6, 10));
            Solar2Lunar(D,M,Y);
        }
        if (k == 6){
            cout << "Ban da chon thoat chuong trinh";
            return 0;
        }
    }

    return 0;
}
