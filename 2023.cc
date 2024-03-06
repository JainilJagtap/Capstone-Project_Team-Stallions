#include<iostream>
#include<bits/stdc++.h>
using namespace std;

int main()
{
    int t;
    cin>>t;
    while(t--){
        int n,k;
        cin>>n>>k;
        vector<int>b(n);
        long long int prod=1;
        for(int i=0;i<n;i++){
            cin>>b[i];
            prod*=b[i];
        }
        
        if(2023%prod==0){
            cout<<"YES"<<endl;
            cout<<(2023/prod)<<" ";
            for(int i=0;i<k-1;i++) cout<<1<<" ";
            cout<<"\n";
        }
        else{
            cout<<"NO"<<endl;
        }
    }
    return 0;
}