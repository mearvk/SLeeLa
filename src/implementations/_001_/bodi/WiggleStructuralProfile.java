package implementations._001_.bodi;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.EnumMap;
import java.util.List;
import java.util.Map;

/**
 * Evidence-oriented result of a Wiggle structural inference pass.
 * Confidence values are hypotheses, not truth claims.
 */
public final class WiggleStructuralProfile
{
    private final EnumMap<WiggleModel, Double> confidence;
    private final int elementCount;
    private final int attributeCount;
    private final int maxDepth;
    private final int repeatedElementKinds;
    private final int referenceCandidates;
    private final int searchCount;

    WiggleStructuralProfile(EnumMap<WiggleModel, Double> confidence,
                            int elementCount,
                            int attributeCount,
                            int maxDepth,
                            int repeatedElementKinds,
                            int referenceCandidates,
                            int searchCount)
    {
        this.confidence = new EnumMap<WiggleModel, Double>(confidence);
        this.elementCount = elementCount;
        this.attributeCount = attributeCount;
        this.maxDepth = maxDepth;
        this.repeatedElementKinds = repeatedElementKinds;
        this.referenceCandidates = referenceCandidates;
        this.searchCount = searchCount;
    }

    public double confidence(WiggleModel model)
    {
        Double value = confidence.get(model);
        return value == null ? 0.0 : value.doubleValue();
    }

    public Map<WiggleModel, Double> confidence()
    {
        return Collections.unmodifiableMap(confidence);
    }

    public List<WiggleModel> rankedModels()
    {
        List<WiggleModel> result = new ArrayList<WiggleModel>(confidence.keySet());
        Collections.sort(result, new Comparator<WiggleModel>()
        {
            public int compare(WiggleModel left, WiggleModel right)
            {
                return Double.compare(confidence(right), confidence(left));
            }
        });
        return result;
    }

    public WiggleModel leadingModel()
    {
        List<WiggleModel> ranked = rankedModels();
        return ranked.isEmpty() ? null : ranked.get(0);
    }

    public int elementCount() { return elementCount; }
    public int attributeCount() { return attributeCount; }
    public int maxDepth() { return maxDepth; }
    public int repeatedElementKinds() { return repeatedElementKinds; }
    public int referenceCandidates() { return referenceCandidates; }
    public int searchCount() { return searchCount; }

    /** Returns a compact, provenance-friendly accounting record. */
    public String accounting()
    {
        return "models=" + rankedModels()
            + ", elements=" + elementCount
            + ", attributes=" + attributeCount
            + ", depth=" + maxDepth
            + ", repeatedKinds=" + repeatedElementKinds
            + ", references=" + referenceCandidates
            + ", searches=" + searchCount;
    }
}
