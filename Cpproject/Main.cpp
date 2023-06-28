#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include <mutex>
#include <queue>
#include <thread>

std::mutex dataMutex;

using json = nlohmann::json;

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

void fetchdata(const std::string& url, std::priority_queue<std::pair<double,std::string>>& pq,const std::string& exchange, const std::string& target) {
    CURL* curl = curl_easy_init();
    if (curl) {
        
      
        std::string v;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &v);
        

        CURLcode res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            
            json jsonResponse = json::parse(v);

            

            for (const auto& i : jsonResponse["tickers"]) {
                if (target == i["target"]) {
                    std::lock_guard<std::mutex> lock(dataMutex);
                      pq.push({i["last"],exchange}) ;

                    break;
                }
            }
        } else {
            std::cerr << "Request failed: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
    }
}

int main() {
        std::string token , target ;
        std::cout<<"Token you want to Trade ";
        std::cin>>token;
        std::cout<<"against Target(Ex.USDT,BUSD)";
        std::cin>>target;
        std::vector<std::string> exchng {"gdax","binance_us","binance","bitfinex"};
        std::priority_queue<std::pair<double, std::string>> pq;
        std::vector<std::thread> threads;

        for (const std::string& exchange : exchng) {
            
            std::string url="https://api.coingecko.com/api/v3/coins/"+token+"/tickers?exchange_ids="+exchange;
            threads.emplace_back(fetchdata, url, std::ref(pq), exchange, target);
        }

        for (std::thread& thread : threads) {
            thread.join();
        }

        double maxVal = pq.top().first;
        double minVal = pq.top().first;
        std::string maxName = pq.top().second;
        std::string minName = pq.top().second;

        while (!pq.empty()) {
            double data = pq.top().first;
            std::cout<<data<<" "<<pq.top().second<<"\n";
            pq.pop();
            if (data > maxVal) {
            maxVal = data;
            maxName = pq.top().second;
         }

            if (data < minVal) {
            minVal = data;
            minName = pq.top().second;
         }
         
    }

        

        std::cout<<"Best Trade is between "<<maxName<<" and "<<minName<<" of difference: "<<maxVal-minVal<< std::endl;;
        std::cout<<"Happy Trading!"<< std::endl;;

        return 0;
}