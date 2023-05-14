function D = MPPT (Upv, Ipv, step)

persistent Irev Urev Drev
    
if isempty(Irev)
    Drev = 0.1;
    Urev = 0;
    Irev = 0;
end
    
    
dI = Ipv - Irev;
dU = Upv - Urev;
    
if dU ~= 0
    if Ipv + dI / dU * Upv ~= 0
        if Ipv + dI / dU * Upv > 0
            D = Drev - step;
        else
            D = Drev + step;
        end
    else
            D = Drev;
      end
else
    if dI ~= 0
        if dI > 0
            D = Drev + step;
        else
            D = Drev - step;
        end
    else
        D = Drev;
        end
end
    
Drev = D;
Irev = Ipv;
Urev = Upv;

end