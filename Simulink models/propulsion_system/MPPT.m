function [D, N, stepp]= MPPT(Upv, Ipv, steppp)

persistent Prev1 Prev2 Urev1 Urev2 Drev Drev1 Drev2 step1 step2 Dbest Dbest1 Dbest2 Pbest Pbest1 Pbest2

if isempty(Prev1)
    
    
    
end

P = Upv * Ipv;

if P == 0
    Drev = 0.2;
end

if n == 1
    
    dP = P - Prev1;
    dU = U - Urev1;
    step = 0.4 * step1 + 2 * (Dbest1 - Drev1) + 2 * (Dbest - Drev1);
    step1 = step;
    
    if P > Pbest1
        Dbest1 = Drev1;
        Pbest1 = P;
    end
    
    if dP > 0
        if dU > 0 
            Drev = Drev1 + step1;        
        else
            Drev = Drev1 - step1;
        end
    else
        if dU > 0 
            Drev = Drev1 - step1;
        else
            Drev = Drev1 + step1;
        end
    end
    
    Drev1 = Drev;
    Urev1 = U;
    Prev1 = P;
end

if n == 2
    
    dP = P - Prev2;
    dU = U - Urev2;
    step = 0.4 * step2 + 2 * (Dbest2 - Drev2) + 2 * (Dbest - Drev2);
    step2 = step;
    
    if P > Pbest2
        Dbest2 = Drev2;
        Pbest2 = P;
    end
    
    if dP > 0
        if dU > 0 
            Drev = Drev2 + step2;
        else
            Drev = Drev2 - step2;
        end
    else
        if dU > 0 
            Drev = Drev2 - step2;
        else
            Drev = Drev2 + step2;
        end
    end

	Drev2 = Drev;
    Urev2 = U;
    Prev2 = P;
end

n = n + 1;

if n == 3
    n = 1;
end

if n == 1 
    Drev = Drev2;
end

if n == 2
    Drev = Drev1;
end

if Pbest < Pbest1
    Pbest = Pbest1;
    Dbest = Dbest1;
end

if Pbest < Pbest2
    Pbest = Pbest2;
    Dbest = Dbest2;
end

steppp = step1;
D = Drev;
N = n;
end