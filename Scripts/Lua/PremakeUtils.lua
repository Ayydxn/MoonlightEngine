function getProjectBinariesDir(projectName)
    return "%{wks.location}/Binaries/" .. projectName .. "/%{cfg.buildcfg}-" .. firstToUpper(os.target()) .. "-%{cfg.architecture}"
end

function getProjectIntermediateDir(projectName)
    return "%{wks.location}/Intermediate/" .. projectName .. "/%{cfg.buildcfg}-" .. firstToUpper(os.target()) .. "-%{cfg.architecture}"
end
