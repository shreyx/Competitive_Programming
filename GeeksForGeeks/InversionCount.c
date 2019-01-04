// Solution for Inversion Count in an array.
// Sorted Output Returns 0
// Reverse Sorted return n*(n+1)

// Approach Used Merge Sort Divide & Conquer

#include<stdio.h>
int a[10000000];
long long countInversion(int l,int r);

int main() 
{
    int t,n,i;
    
    scanf("%d",&t);
    while(t--)
    {
        scanf("%d",&n);
        for(i=0;i<n;i++)
            scanf("%d",&a[i]);
        long long ans = countInversion(0,n-1);
        printf("%lld\n",ans);
    }
    return 0;
}

long long countInversion(int l , int r)
{
    if(l==r)
        return 0;
    int mid = l + (r-l)/2;
    long long inversions_left = countInversion(l,mid);
    long long inversions_right = countInversion(mid+1, r);
    
    int i,j,k;
    
    long long inversions = inversions_left + inversions_right;
    
    int ln = mid - l + 1;
    int rn = r - mid;
    
    int left[ln] , right[rn];
    
    for(i=l ; i<=mid ; i++)
        left[i-l]=a[i];
        
    for(i=mid+1 ; i<=r ; i++)
        right[i-mid - 1 ] = a[i];
        
    i = 0;
    j = 0;
    k = l;
    
    while(i < ln && j < rn)
    {
        if(left[i] <= right[j])
            a[k++]=left[i++];
        else
        {
            a[k++]=right[j++];
            inversions+=(ln - i);   // Core Important Step
        }
    }
    while(i< ln)
        a[k++]=left[i++];
    while(j< rn)  
        a[k++]=right[j++];
        
    return inversions;
}
