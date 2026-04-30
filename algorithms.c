#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_line(char c, int n) {
    for (int i = 0; i < n; i++) putchar(c);
    putchar('\n');
}

void print_header(const char *title, const char *type) {
    printf("\n");
    print_line('=', 58);
    printf("  [%s]  %s\n", type, title);
    print_line('=', 58);
}

int imax(int a, int b) { return a > b ? a : b; }
int imin(int a, int b) { return a < b ? a : b; }

// ============================================================
//  GREEDY 1 — COIN CHANGE
// ============================================================
void coin_change() {
    print_header("Coin Change", "GREEDY");
    int denominations[] = {25, 10, 5, 1};
    int num_coins = 4, amount;
    printf("  Denominations: 25c  10c  5c  1c\n");
    printf("  Enter amount (cents): ");
    scanf("%d", &amount);
    if (amount <= 0) { printf("  [!] Amount must be positive.\n"); return; }

    int used[4] = {0}, remaining = amount;
    printf("\n  Steps:\n");
    print_line('-', 45);
    for (int i = 0; i < num_coins; i++) {
        used[i] = remaining / denominations[i];
        remaining %= denominations[i];
        if (used[i] > 0)
            printf("  Use %2d x %2dc  -->  remaining = %d\n", used[i], denominations[i], remaining);
    }
    print_line('-', 45);
    if (remaining == 0) {
        int total = 0;
        for (int i = 0; i < num_coins; i++) total += used[i];
        printf("  Result: %dc using %d coin(s)\n  Coins :", amount, total);
        for (int i = 0; i < num_coins; i++)
            for (int j = 0; j < used[i]; j++) printf(" [%dc]", denominations[i]);
        printf("\n");
    } else {
        printf("  [!] Exact change not possible.\n");
    }
}

// ============================================================
//  GREEDY 2 — FRACTIONAL KNAPSACK
// ============================================================
void fractional_knapsack() {
    print_header("Fractional Knapsack", "GREEDY");
    int n, capacity;
    char names[20][30];
    double weights[20], values[20], ratio[20];

    printf("  Number of items: "); scanf("%d", &n);
    if (n <= 0 || n > 20) { printf("  [!] Invalid.\n"); return; }
    printf("  Knapsack capacity: "); scanf("%d", &capacity);

    printf("\n  Enter items (name weight value):\n");
    for (int i = 0; i < n; i++) {
        printf("  Item %d: ", i+1);
        scanf("%s %lf %lf", names[i], &weights[i], &values[i]);
        ratio[i] = values[i] / weights[i];
    }

    // Bubble sort by ratio descending
    for (int i = 0; i < n-1; i++)
        for (int j = 0; j < n-i-1; j++)
            if (ratio[j] < ratio[j+1]) {
                double tmp; char tmps[30];
                tmp=ratio[j];   ratio[j]=ratio[j+1];     ratio[j+1]=tmp;
                tmp=weights[j]; weights[j]=weights[j+1]; weights[j+1]=tmp;
                tmp=values[j];  values[j]=values[j+1];   values[j+1]=tmp;
                strcpy(tmps,names[j]); strcpy(names[j],names[j+1]); strcpy(names[j+1],tmps);
            }

    printf("\n  Sorted by value/weight ratio:\n");
    print_line('-', 52);
    printf("  %-12s %8s %8s %10s\n", "Item","Weight","Value","Ratio");
    print_line('-', 52);
    for (int i = 0; i < n; i++)
        printf("  %-12s %8.2f %8.2f %10.4f\n", names[i], weights[i], values[i], ratio[i]);
    print_line('-', 52);

    double remaining = capacity, total_val = 0;
    printf("\n  Packing:\n");
    print_line('-', 52);
    for (int i = 0; i < n && remaining > 0; i++) {
        double take = (weights[i] <= remaining) ? weights[i] : remaining;
        double gained = take * ratio[i];
        total_val += gained;
        remaining -= take;
        if (take == weights[i])
            printf("  [FULL] %-10s  take %.2f kg  value = %.2f\n", names[i], take, gained);
        else
            printf("  [FRAC] %-10s  take %.2f/%.2f kg  value = %.2f\n", names[i], take, weights[i], gained);
    }
    print_line('-', 52);
    printf("  Total value  : %.4f\n  Remaining cap: %.2f\n", total_val, remaining);
}

// ============================================================
//  GREEDY 3 — ACTIVITY SELECTION
// ============================================================
#define MAX_ACT 20
typedef struct { char name[10]; int start, end; } Activity;
int cmp_end(const void *a, const void *b) {
    return ((Activity*)a)->end - ((Activity*)b)->end;
}

void activity_selection() {
    print_header("Activity Selection", "GREEDY");
    int n; Activity acts[MAX_ACT];
    printf("  Number of activities (max %d): ", MAX_ACT); scanf("%d", &n);
    if (n <= 0 || n > MAX_ACT) { printf("  [!] Invalid.\n"); return; }

    printf("\n  Enter activities (name start end):\n");
    for (int i = 0; i < n; i++) {
        printf("  Activity %d: ", i+1);
        scanf("%s %d %d", acts[i].name, &acts[i].start, &acts[i].end);
    }
    qsort(acts, n, sizeof(Activity), cmp_end);

    printf("\n  Sorted by finish time:\n");
    print_line('-', 40);
    printf("  %-8s %-8s %-8s\n","Name","Start","End");
    print_line('-', 40);
    for (int i = 0; i < n; i++)
        printf("  %-8s %-8d %-8d\n", acts[i].name, acts[i].start, acts[i].end);
    print_line('-', 40);

    int selected[MAX_ACT], count=0, last_end=-1;
    printf("\n  Selection:\n"); print_line('-', 50);
    for (int i = 0; i < n; i++) {
        if (acts[i].start >= last_end) {
            selected[count++] = i;
            printf("  [+] %-6s (start=%d, end=%d) --> SELECTED\n", acts[i].name, acts[i].start, acts[i].end);
            last_end = acts[i].end;
        } else {
            printf("  [-] %-6s (start=%d < last_end=%d) --> SKIPPED\n", acts[i].name, acts[i].start, last_end);
        }
    }
    print_line('-', 50);
    printf("  Total selected: %d  --> ", count);
    for (int i = 0; i < count; i++) printf("%s%s", acts[selected[i]].name, i<count-1?", ":"\n");
}

// ============================================================
//  GREEDY 4 — JOB SEQUENCING WITH DEADLINES
// ============================================================
typedef struct { char name[10]; int deadline, profit; } Job;
int cmp_profit(const void *a, const void *b) {
    return ((Job*)b)->profit - ((Job*)a)->profit;
}

void job_sequencing() {
    print_header("Job Sequencing with Deadlines", "GREEDY");
    int n; Job jobs[20];
    printf("  Each job takes 1 unit of time.\n");
    printf("  Number of jobs: "); scanf("%d", &n);
    if (n <= 0 || n > 20) { printf("  [!] Invalid.\n"); return; }

    printf("\n  Enter jobs (name deadline profit):\n");
    for (int i = 0; i < n; i++) {
        printf("  Job %d: ", i+1); scanf("%s %d %d", jobs[i].name, &jobs[i].deadline, &jobs[i].profit);
    }
    qsort(jobs, n, sizeof(Job), cmp_profit);

    int max_d = 0;
    for (int i = 0; i < n; i++) if (jobs[i].deadline > max_d) max_d = jobs[i].deadline;

    int slots[21] = {0}, slot_job[21];
    memset(slot_job, -1, sizeof(slot_job));
    int total_profit = 0, count = 0;

    printf("\n  Assigning jobs (sorted by profit desc):\n"); print_line('-', 52);
    for (int i = 0; i < n; i++) {
        int placed = 0;
        for (int s = jobs[i].deadline; s >= 1; s--) {
            if (!slots[s]) {
                slots[s]=1; slot_job[s]=i;
                total_profit += jobs[i].profit; count++; placed=1;
                printf("  [+] %-6s  deadline=%d  profit=%3d  --> slot %d\n", jobs[i].name, jobs[i].deadline, jobs[i].profit, s);
                break;
            }
        }
        if (!placed)
            printf("  [-] %-6s  deadline=%d  profit=%3d  --> NO SLOT\n", jobs[i].name, jobs[i].deadline, jobs[i].profit);
    }
    print_line('-', 52);
    printf("  Schedule: ");
    for (int s = 1; s <= max_d; s++) if (slot_job[s] != -1) printf("[Slot %d:%s] ", s, jobs[slot_job[s]].name);
    printf("\n  Jobs done   : %d\n  Total profit: %d\n", count, total_profit);
}

// ============================================================
//  DP 1 — 0/1 KNAPSACK
// ============================================================
#define MAX_ITEMS    20
#define MAX_CAPACITY 50

void knapsack() {
    print_header("0/1 Knapsack", "DP");
    int n, capacity, weights[MAX_ITEMS], values[MAX_ITEMS];
    char names[MAX_ITEMS][30];

    printf("  Number of items (max %d): ", MAX_ITEMS); scanf("%d", &n);
    if (n<=0||n>MAX_ITEMS){printf("  [!] Invalid.\n");return;}
    printf("  Knapsack capacity (max %d): ", MAX_CAPACITY); scanf("%d", &capacity);
    if (capacity<=0||capacity>MAX_CAPACITY){printf("  [!] Invalid.\n");return;}

    printf("\n  Enter items (name weight value):\n");
    for (int i = 0; i < n; i++) {
        printf("  Item %d: ", i+1); scanf("%s %d %d", names[i], &weights[i], &values[i]);
    }

    int dp[MAX_ITEMS+1][MAX_CAPACITY+1];
    memset(dp, 0, sizeof(dp));
    for (int i = 1; i <= n; i++)
        for (int w = 0; w <= capacity; w++) {
            dp[i][w] = dp[i-1][w];
            if (weights[i-1] <= w) {
                int with = dp[i-1][w-weights[i-1]] + values[i-1];
                if (with > dp[i][w]) dp[i][w] = with;
            }
        }

    int show = imin(capacity, 10);
    printf("\n  DP Table (cols 0..%d):\n", show);
    print_line('-', 50);
    printf("  %-12s", "Item\\W");
    for (int w = 0; w <= show; w++) printf("%4d", w);
    printf("\n"); print_line('-', 50);
    printf("  %-12s", "(none)");
    for (int w = 0; w <= show; w++) printf("%4d", 0);
    printf("\n");
    for (int i = 1; i <= n; i++) {
        printf("  %-12s", names[i-1]);
        for (int w = 0; w <= show; w++) printf("%4d", dp[i][w]);
        printf("\n");
    }
    print_line('-', 50);

    printf("\n  Selected items (backtrack):\n");
    int w=capacity, tw=0, tv=0;
    for (int i = n; i > 0; i--) {
        if (dp[i][w] != dp[i-1][w]) {
            printf("  [+] %-12s  weight:%2d  value:%3d\n", names[i-1], weights[i-1], values[i-1]);
            tw+=weights[i-1]; tv+=values[i-1]; w-=weights[i-1];
        }
    }
    print_line('-', 40);
    printf("  Max Value: %d\n  Weight   : %d / %d\n", dp[n][capacity], tw, capacity);
}

// ============================================================
//  DP 2 — LONGEST COMMON SUBSEQUENCE
// ============================================================
void lcs() {
    print_header("Longest Common Subsequence (LCS)", "DP");
    char s1[50], s2[50];
    printf("  Enter string 1: "); scanf("%s", s1);
    printf("  Enter string 2: "); scanf("%s", s2);
    int m=strlen(s1), n=strlen(s2), dp[51][51];
    memset(dp, 0, sizeof(dp));
    for (int i=1;i<=m;i++)
        for (int j=1;j<=n;j++)
            dp[i][j] = (s1[i-1]==s2[j-1]) ? dp[i-1][j-1]+1 : imax(dp[i-1][j],dp[i][j-1]);

    printf("\n  DP Table:\n");
    int cols = imin(n, 12);
    print_line('-', 6+cols*4);
    printf("  %4c", ' '); printf("%4c",'-');
    for (int j=0;j<cols;j++) printf("%4c",s2[j]); printf("\n");
    print_line('-', 6+cols*4);
    for (int i=0;i<=m;i++){
        printf("  %4c", i==0?'-':s1[i-1]);
        for (int j=0;j<=cols;j++) printf("%4d",dp[i][j]);
        printf("\n");
    }
    print_line('-', 6+cols*4);

    // Backtrack
    char lcs_str[51]; int len=0, ci=m, cj=n;
    while (ci>0&&cj>0) {
        if (s1[ci-1]==s2[cj-1]){lcs_str[len++]=s1[ci-1];ci--;cj--;}
        else if (dp[ci-1][cj]>dp[ci][cj-1]) ci--;
        else cj--;
    }
    for (int a=0,b=len-1;a<b;a++,b--){char t=lcs_str[a];lcs_str[a]=lcs_str[b];lcs_str[b]=t;}
    lcs_str[len]='\0';
    printf("  LCS Length : %d\n  LCS String : \"%s\"\n", dp[m][n], lcs_str);
}

// ============================================================
//  DP 3 — LONGEST INCREASING SUBSEQUENCE
// ============================================================
void lis() {
    print_header("Longest Increasing Subsequence (LIS)", "DP");
    int n, arr[50], dp[50], parent[50];
    printf("  Number of elements (max 50): "); scanf("%d",&n);
    if (n<=0||n>50){printf("  [!] Invalid.\n");return;}
    printf("  Enter elements: ");
    for (int i=0;i<n;i++) scanf("%d",&arr[i]);

    for (int i=0;i<n;i++){dp[i]=1;parent[i]=-1;}
    for (int i=1;i<n;i++)
        for (int j=0;j<i;j++)
            if (arr[j]<arr[i] && dp[j]+1>dp[i]){dp[i]=dp[j]+1;parent[i]=j;}

    int best=0;
    for (int i=1;i<n;i++) if (dp[i]>dp[best]) best=i;

    printf("\n  dp[] values (LIS length ending at each index):\n");
    print_line('-', 38);
    for (int i=0;i<n;i++) printf("  arr[%d] = %-4d  LIS ending here = %d\n", i, arr[i], dp[i]);
    print_line('-', 38);

    // Backtrack path
    int path[50], plen=0, cur=best;
    while (cur!=-1){path[plen++]=cur;cur=parent[cur];}
    printf("  LIS Length : %d\n  LIS        :", dp[best]);
    for (int i=plen-1;i>=0;i--) printf(" %d", arr[path[i]]);
    printf("\n");
}

// ============================================================
//  DP 4 — MATRIX CHAIN MULTIPLICATION
// ============================================================
int g_split[11][11];

void print_order(int i, int j) {
    if (i==j) {printf("M%d",i); return;}
    printf("(");
    print_order(i, g_split[i][j]);
    printf(" x ");
    print_order(g_split[i][j]+1, j);
    printf(")");
}

void matrix_chain() {
    print_header("Matrix Chain Multiplication", "DP");
    int n, dims[15];
    printf("  Number of matrices (max 10): "); scanf("%d",&n);
    if (n<=0||n>10){printf("  [!] Invalid.\n");return;}
    printf("  Enter %d dimensions (e.g. A[2x3]B[3x4] -> 2 3 4):\n  ", n+1);
    for (int i=0;i<=n;i++) scanf("%d",&dims[i]);

    int dp[11][11];
    memset(dp,0,sizeof(dp));
    memset(g_split,0,sizeof(g_split));

    for (int len=2;len<=n;len++)
        for (int i=1;i<=n-len+1;i++) {
            int j=i+len-1;
            dp[i][j]=1<<30;
            for (int k=i;k<j;k++){
                int cost=dp[i][k]+dp[k+1][j]+dims[i-1]*dims[k]*dims[j];
                if (cost<dp[i][j]){dp[i][j]=cost;g_split[i][j]=k;}
            }
        }

    printf("\n  Cost table (min scalar multiplications):\n");
    print_line('-', 48);
    printf("  %4s","");
    for (int j=1;j<=n;j++) printf("  M%d ",j); printf("\n");
    for (int i=1;i<=n;i++){
        printf("  M%d  ",i);
        for (int j=1;j<=n;j++){
            if (j<i) printf("   -  ");
            else     printf("%5d ", dp[i][j]);
        }
        printf("\n");
    }
    print_line('-', 48);
    printf("  Min multiplications: %d\n  Optimal order      : ", dp[1][n]);
    print_order(1,n);
    printf("\n");
}

// ============================================================
//  DP 5 — FIBONACCI WITH MEMOIZATION
// ============================================================
long long fib_memo_table[100];

long long fib_calc(int n) {
    if (n<=1) return n;
    if (fib_memo_table[n]!=-1) return fib_memo_table[n];
    fib_memo_table[n] = fib_calc(n-1) + fib_calc(n-2);
    return fib_memo_table[n];
}

void fibonacci_dp() {
    print_header("Fibonacci (Top-Down DP / Memoization)", "DP");
    int n;
    printf("  Enter n (compute F(0)..F(n), max 50): "); scanf("%d",&n);
    if (n<0||n>50){printf("  [!] Invalid.\n");return;}
    memset(fib_memo_table,-1,sizeof(fib_memo_table));

    printf("\n  Computing step by step:\n");
    print_line('-', 46);
    printf("  %-8s %-20s %s\n","n","F(n)","Status");
    print_line('-', 46);
    for (int i=0;i<=n;i++){
        int was_cached=(fib_memo_table[i]!=-1);
        long long val=fib_calc(i);
        printf("  F(%-3d)  %-20lld  %s\n", i, val, was_cached?"[cache hit]":"[computed]");
    }
    print_line('-', 46);
    printf("  F(%d) = %lld\n", n, fib_memo_table[n]);
}

// ============================================================
//  DP 6 — CLOTH CUTTING (ROD CUTTING)
// ============================================================
void cloth_cutting() {
    print_header("Cloth Cutting (Rod Cutting)", "DP");
    int n;
    printf("  Number of available piece lengths (max 50): "); scanf("%d", &n);
    if (n <= 0 || n > 50) { printf("  [!] Invalid.\n"); return; }

    int price[51];
    for (int i = 1; i <= n; i++) price[i] = 0;
    printf("  Enter prices for lengths 1..%d:\n", n);
    for (int i = 1; i <= n; i++) {
        printf("  price[%d]: ", i); scanf("%d", &price[i]);
    }

    int L;
    printf("  Enter total cloth length to cut: "); scanf("%d", &L);
    if (L <= 0 || L > 100) { printf("  [!] Invalid length (1..100).\n"); return; }

    int dp[101]; // dp[len] = max revenue
    int cut[101]; // cut[len] = first piece length used
    for (int i = 0; i <= L; i++) { dp[i] = 0; cut[i] = 0; }

    for (int len = 1; len <= L; len++) {
        dp[len] = 0; cut[len] = 0;
        for (int piece = 1; piece <= n && piece <= len; piece++) {
            int candidate = price[piece] + dp[len - piece];
            if (candidate > dp[len]) { dp[len] = candidate; cut[len] = piece; }
        }
    }

    int show = imin(L, 10);
    printf("\n  DP results (len 0..%d):\n", show);
    print_line('-', 48);
    printf("  len :");
    for (int i = 0; i <= show; i++) printf(" %3d", i);
    printf("\n  val :");
    for (int i = 0; i <= show; i++) printf(" %3d", dp[i]);
    printf("\n");
    print_line('-', 48);

    printf("\n  Optimal cutting for length %d:\n", L);
    print_line('-', 40);
    int rem = L, total = dp[L];
    while (rem > 0) {
        int p = cut[rem];
        if (p == 0) {
            printf("  [!] Cannot cut remaining length %d\n", rem); break;
        }
        printf("  Cut piece: %d  (price=%d)\n", p, price[p]);
        rem -= p;
    }
    print_line('-', 40);
    printf("  Max revenue: %d\n", total);
}

// ============================================================
//  MAIN MENU
// ============================================================
int main() {
    int choice;
    printf("\n");
    print_line('*', 60);
    printf("*%*s*\n",58,"");
    printf("*    ALGORITHM VISUALIZER  --  CCC Project%*s*\n",17,"");
    printf("*    4x Greedy  +  6x Dynamic Programming%*s*\n",18,"");
    printf("*%*s*\n",58,"");
    print_line('*', 60);

    do {
        printf("\n  --- GREEDY ALGORITHMS ---\n");
        printf("  1.  Coin Change\n");
        printf("  2.  Fractional Knapsack\n");
        printf("  3.  Activity Selection\n");
        printf("  4.  Job Sequencing with Deadlines\n");
        printf("\n  --- DYNAMIC PROGRAMMING ---\n");
        printf("  5.  0/1 Knapsack\n");
        printf("  6.  Longest Common Subsequence (LCS)\n");
        printf("  7.  Longest Increasing Subsequence (LIS)\n");
        printf("  8.  Matrix Chain Multiplication\n");
        printf("  9.  Fibonacci (Memoization)\n");
        printf("  10. Cloth Cutting (Rod Cutting)\n");
        printf("\n  0.  Exit\n");
        print_line('-', 40);
        printf("  Choice: ");
        scanf("%d",&choice);

        switch(choice){
            case 1: coin_change();          break;
            case 2: fractional_knapsack();  break;
            case 3: activity_selection();   break;
            case 4: job_sequencing();       break;
            case 5: knapsack();             break;
            case 6: lcs();                  break;
            case 7: lis();                  break;
            case 8: matrix_chain();         break;
            case 9: fibonacci_dp();         break;
            case 10: cloth_cutting();       break;
            case 0: printf("\n  Goodbye.\n\n"); break;
            default: printf("  [!] Invalid option.\n");
        }
    } while(choice!=0);
    return 0;
}
